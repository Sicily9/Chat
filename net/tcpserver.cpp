/*
 * tcpserver.cpp
 *
 *  Created on: 2018年3月21日
 *      Author: xc
 */
#include "tcpserver.h"
#include "acceptor.h"
#include "reactor.h"
#include "reactorthreadpool.h"
#include "../utility/log/log.h"
#include "socketop.h"
#include <stdio.h>

using namespace xc;
using namespace xc::net;

TcpServer::TcpServer(EventLoop* loop,
                     const InetAddress& listenAddr,
                     const std::string& nameArg,
                     Option option)
  : _loop(loop),
    _hostport(listenAddr.toIpPort()),
    _name(nameArg),
    _acceptor(new Acceptor(loop, listenAddr, option == kReusePort)),
    _threadPool(new EventLoopThreadPool(loop)),
    _connectionCallback(),
    _messageCallback(),
    _nextConnId(1)
{
	_acceptor->setNewConnectionCallback([this](int sockfd, const InetAddress& inetaddr){
		this->newConnection(sockfd,inetaddr);
	});
}

TcpServer::~TcpServer()
{
	XC_LOG_DEBUG("server down");
	for (ConnectionMap::iterator it(_connections.begin()); it != _connections.end(); ++it)
	{
		TcpConnectionPtr conn = it->second;
		it->second.reset();
		conn->getLoop()->runInLoop([conn](){
			conn->connectDestroyed();
		});
		conn.reset();
	}
}

void TcpServer::setThreadNum(int numThreads)
{
	assert(0 <= numThreads);
	_threadPool->setThreadNum(numThreads);
}

void TcpServer::start()
{
	XC_LOG_DEBUG("server start ");
	_threadPool->start(_threadInitCallback);
	_loop->runInLoop([this](){
		this->_acceptor->listen();
	});

}

void TcpServer::newConnection(int sockfd, const InetAddress& peerAddr)
{
	XC_LOG_DEBUG("new connection");
	EventLoop* ioLoop = _threadPool->getNextLoop();
	char buf[32];
	snprintf(buf, sizeof buf, ":%s#%d", _hostport.c_str(), _nextConnId);
	++_nextConnId;
	std::string connName = _name + buf;

	InetAddress localAddr(socketsop::getLocalAddr(sockfd));

	TcpConnectionPtr conn(new TcpConnection(ioLoop, connName, sockfd,localAddr, peerAddr) );
	_connections[connName] = conn;
	conn->setConnectionCallback(_connectionCallback);
	conn->setMessageCallback(_messageCallback);
	conn->setWriteCompleteCallback(_writeCompleteCallback);
	conn->setCloseCallback([this](const TcpConnectionPtr& conn){
		this->removeConnection(conn);
	});
	ioLoop->runInLoop([conn](){
		conn->connectEstablished();
	});
}

void TcpServer::removeConnection(const TcpConnectionPtr& conn)
{
	_loop->runInLoop([this,conn](){
		this->removeConnectionInLoop(conn);
	});
}

void TcpServer::removeConnectionInLoop(const TcpConnectionPtr& conn)
{
	size_t n = _connections.erase(conn->name());
	(void)n;
	assert(n == 1);
	EventLoop* ioLoop = conn->getLoop();
	ioLoop->queueInLoop([conn](){
		conn->connectDestroyed();
	});
}





