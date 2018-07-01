/*
 * tcpconnection.h
 *
 *  Created on: 2018年3月21日
 *      Author: xc
 */

#ifndef NET_TCPCONNECTION_H_
#define NET_TCPCONNECTION_H_

#include "../utility/buffer/buffer.h"
#include "inetaddr.h"
#include <string>
#include <boost/any.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

using namespace xc::base;
namespace xc{
	namespace net{
		class Handler;
		class EventLoop;
		class Socket;
		class TcpConnection;
		typedef boost::shared_ptr<TcpConnection> TcpConnectionPtr;
		typedef boost::function<void()> TimerCallback;
		typedef boost::function<void (const TcpConnectionPtr&)> ConnectionCallback;
		typedef boost::function<void (const TcpConnectionPtr&)> CloseCallback;
		typedef boost::function<void (const TcpConnectionPtr&)> WriteCompleteCallback;
		typedef boost::function<void (const TcpConnectionPtr&, size_t)> HighWaterMarkCallback;
		typedef boost::function<void (const TcpConnectionPtr&, Buffer*)> MessageCallback;
		class TcpConnection : boost::noncopyable,
		                      public boost::enable_shared_from_this<TcpConnection>
		{
		 public:
		  /// Constructs a TcpConnection with a connected sockfd
		  ///
		  /// User should not create this object.
		  TcpConnection(EventLoop* loop,
		                const std::string& name,
		                int sockfd,
		                const InetAddress& localAddr,
		                const InetAddress& peerAddr);
		  ~TcpConnection();

		  EventLoop* getLoop() const { return _loop; }
		  const std::string& name() const { return _name; }
		  const InetAddress& localAddress() const { return _localAddr; }
		  const InetAddress& peerAddress() const { return _peerAddr; }
		  bool connected() const { return _state == kConnected; }
		  Handler* getHandler() const { return &*_handler; }
		  // void send(string&& message); // C++11
		  void send(const void* message, int len);
		  void send(const std::string& message);
		  // void send(Buffer&& message); // C++11
		  void send(Buffer* message);  // this one will swap data
		  void shutdown(); // NOT thread safe, no simultaneous calling
		  // void shutdownAndForceCloseAfter(double seconds); // NOT thread safe, no simultaneous calling
		  void forceClose();
		  void forceCloseWithDelay(double seconds);
		  void setTcpNoDelay(bool on);

		  void setContext(const boost::any& context)
		  { _context = context; }

		  const boost::any& getContext() const
		  { return _context; }

		  boost::any* getMutableContext()
		  { return &_context; }

		  void setConnectionCallback(const ConnectionCallback& cb)
		  { _connectionCallback = cb; }

		  void setMessageCallback(const MessageCallback& cb)
		  { _messageCallback = cb; }

		  void setWriteCompleteCallback(const WriteCompleteCallback& cb)
		  { _writeCompleteCallback = cb; }

		  void setHighWaterMarkCallback(const HighWaterMarkCallback& cb, size_t highWaterMark)
		  { _highWaterMarkCallback = cb; _highWaterMark = highWaterMark; }

		  /// Advanced interface
		  Buffer* inputBuffer()
		  { return &_inputBuffer; }

		  Buffer* outputBuffer()
		  { return &_outputBuffer; }

		  /// Internal use only.
		  void setCloseCallback(const CloseCallback& cb){ _closeCallback = cb; }

		  // called when TcpServer accepts a new connection
		  void connectEstablished();   // should be called only once
		  // called when TcpServer has removed me from its map
		  void connectDestroyed();  // should be called only once

		 private:
		  enum StateE { kDisconnected, kConnecting, kConnected, kDisconnecting };
		  void handleRead();
		  void handleWrite();
		  void handleClose();
		  void handleError();
		  // void sendInLoop(string&& message);
		  void sendInLoop(const std::string& message);
		  void sendInLoop(const char* message, size_t len);
		  void shutdownInLoop();
		  // void shutdownAndForceCloseInLoop(double seconds);
		  void forceCloseInLoop();
		  void setState(StateE s) { _state = s; }

		  EventLoop* _loop;
		  const std::string _name;
		  StateE _state;
		  // we don't expose those classes to client.
		  boost::scoped_ptr<Socket> _socket;
		  boost::scoped_ptr<Handler> _handler;
		  const InetAddress _localAddr;
		  const InetAddress _peerAddr;
		  ConnectionCallback _connectionCallback;
		  MessageCallback _messageCallback;
		  WriteCompleteCallback _writeCompleteCallback;
		  HighWaterMarkCallback _highWaterMarkCallback;
		  CloseCallback _closeCallback;
		  size_t _highWaterMark;
		  Buffer _inputBuffer;
		  Buffer _outputBuffer; // FIXME: use list<Buffer> as output buffer.
		  boost::any _context;

		};

	}
}



#endif /* NET_TCPCONNECTION_H_ */
