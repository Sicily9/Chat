/*
 * tcpconnection.cpp
 *
 *  Created on: 2018年3月21日
 *      Author: xc
 */

#include "tcpconnection.h"
#include "handler.h"
#include "reactor.h"
#include "socket.h"
#include "socketop.h"
#include "../utility/log/log.h"
#include <boost/function.hpp>
#include <errno.h>
#include <stdio.h>

using namespace xc;
using namespace xc::net;

TcpConnection::TcpConnection(EventLoop* loop, const std::string& nameArg,
							int sockfd, const InetAddress& localAddr,
                            const InetAddress& peerAddr)
  : _loop(loop),
    _name(nameArg),
    _state(kConnecting),
    _socket(new Socket(sockfd)),
	_handler(new Handler(loop, sockfd)),
    _localAddr(localAddr),
    _peerAddr(peerAddr),
    _highWaterMark(64*1024*1024)
{
	_handler->setReadCallback([this](){
		this->handleRead();
  	});
	_handler->setWriteCallback([this](){
	  	this->handleWrite();
	});
	_handler->setCloseCallback([this](){
	  	this->handleClose();
	});
	_handler->setErrorCallback([this](){
	  	this->handleError();
	});
	_socket->setKeepAlive(false);
}

TcpConnection::~TcpConnection()
{

}

void TcpConnection::send(const void* data, int len)
{
	send(std::string(static_cast<const char*>(data), len));
}

void TcpConnection::send(const std::string& message)
{
	if (_state == kConnected)
	{
		if (_loop->isInLoopThread())
		{
			sendInLoop(message);
		}
		else
		{
			//注意这里点 lamda表达式，message 必须传值 不能引用，引用的话 是不知道这个变量什么时候内存会被释放。
			_loop->runInLoop([this, message](){
				this->sendInLoop(message);
			});
		}
	}
}

void TcpConnection::send(Buffer* buf)
{
	if (_state == kConnected)
	{
		if (_loop->isInLoopThread())
		{
			sendInLoop(buf->peek(), buf->readableBytes());
			buf->retrieveAll();
		}
		else
		{
			_loop->runInLoop([this, buf](){
				this->sendInLoop(buf->retrieveAllAsString());
			});
		}
	}
}

void TcpConnection::sendInLoop(const std::string& message)
{
	sendInLoop(message.c_str(), message.size());
}

void TcpConnection::sendInLoop(const char* data, size_t len)
{

	ssize_t nwrote = 0;
	size_t remaining = len;
	bool faultError = false;
	if (_state == kDisconnected)
		return;
	// if no thing in output queue, try writing directly
	if (!_handler->isWriting() && _outputBuffer.readableBytes() == 0)
	{
		nwrote = socketsop::write(_handler->fd(), data, len);
		if (nwrote >= 0)
		{
			remaining = len - nwrote;
			if (remaining == 0 && _writeCompleteCallback)
			{
				const TcpConnectionPtr ptr = shared_from_this();
				_loop->queueInLoop([ptr,this](){
					this->_writeCompleteCallback(ptr);
				});
			}
		}
		else // nwrote < 0
		{
			XC_LOG_DEBUG("write errno: %d",errno);
			nwrote = 0;
			if (errno != EWOULDBLOCK)
			{
				if (errno == EPIPE || errno == ECONNRESET) // FIXME: any others?
				{
					faultError = true;
				}
			}
		}
	}

	assert(remaining <= len);
	if (!faultError && remaining > 0)
	{
		size_t oldLen = _outputBuffer.readableBytes();
		if (oldLen + remaining >= _highWaterMark
				&& oldLen < _highWaterMark
				&& _highWaterMarkCallback)
		{
			const TcpConnectionPtr ptr = shared_from_this();
			size_t len = oldLen + remaining;
			_loop->queueInLoop([ptr, len, this](){
				this->_highWaterMarkCallback(ptr, len);
			});
		}
		_outputBuffer.append(static_cast<const char*>(data)+nwrote, remaining);
		if (!_handler->isWriting())
		{
			_handler->enableWriting();
		}
	}
}

void TcpConnection::shutdown()
{
	if (_state == kConnected)
	{
		setState(kDisconnecting);
		_loop->runInLoop([this](){
			this->shutdownInLoop();
		});
	}
}

void TcpConnection::shutdownInLoop()
{
	if (!_handler->isWriting())
	{
		// we are not writing
		_socket->shutdownWrite();
	}
}

void TcpConnection::forceClose()
{
	// FIXME: use compare and swap
	if (_state == kConnected || _state == kDisconnecting)
	{
		setState(kDisconnecting);
		const TcpConnectionPtr ptr = shared_from_this();
		_loop->queueInLoop([ptr](){
		ptr->forceCloseInLoop();
	});
	}
}


void TcpConnection::forceCloseInLoop()
{

	if (_state == kConnected || _state == kDisconnecting)
	{
		// as if we received 0 byte in handleRead();
		handleClose();
	}
}

void TcpConnection::setTcpNoDelay(bool on)
{
	_socket->setTcpNoDelay(on);
}

void TcpConnection::connectEstablished()
{
	assert(_state == kConnecting);
	setState(kConnected);
	_handler->tie(shared_from_this());
	_handler->enableReading();
	XC_LOG_DEBUG("new Conncetion  %d", _handler->fd());
	_connectionCallback(shared_from_this());
}

void TcpConnection::connectDestroyed()
{
	if (_state == kConnected)
	{
		setState(kDisconnected);
		_handler->disableAll();
		_connectionCallback(shared_from_this());
	}
	_handler->remove();
}

void TcpConnection::handleRead()
{
	int savedErrno = 0;
		ssize_t n = _inputBuffer.readFd(_handler->fd(), &savedErrno);
	if (n > 0)
	{
		_messageCallback(shared_from_this(), &_inputBuffer);
	}
	else if (n == 0)
	{
		handleClose();
	}
	else
	{
		errno = savedErrno;
		handleError();
	}
}

void TcpConnection::handleWrite()
{
	if (_handler->isWriting())
	{
		ssize_t n = socketsop::write(_handler->fd(),
							   _outputBuffer.peek(),
							   _outputBuffer.readableBytes());
		if (n > 0)
		{
			_outputBuffer.retrieve(n);
			if (_outputBuffer.readableBytes() == 0)
			{
				_handler->disableWriting();
				if (_writeCompleteCallback)
				{
					const TcpConnectionPtr ptr = shared_from_this();
					_loop->queueInLoop([ptr,this](){
						this->_writeCompleteCallback(ptr);
					});
				}
				if (_state == kDisconnecting)
				{
					shutdownInLoop();
				}
			}
		}
		else
		{

		}
	}
	else
	{

	}
}

void TcpConnection::handleClose()
{
	assert(_state == kConnected || _state == kDisconnecting);
	// we don't close fd, leave it to dtor, so we can find leaks easily.
	setState(kDisconnected);
	_handler->disableAll();

	TcpConnectionPtr guardThis(shared_from_this());
	_connectionCallback(guardThis);
	// must be the last line
	_closeCallback(guardThis);
}

void TcpConnection::handleError()
{
	int err = socketsop::getSocketError(_handler->fd());
}




