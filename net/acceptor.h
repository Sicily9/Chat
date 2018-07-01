/*
 * acceptor.h
 *
 *  Created on: 2018年3月13日
 *      Author: xc
 */
#ifndef NET_ACCEPTOR_H_
#define NET_ACCEPTOR_H_

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>

#include "socket.h"
#include "handler.h"
namespace xc{
	namespace net{
		class EventLoop;
		class InetAddress;

		class Acceptor : boost::noncopyable
		{
		 public:
		  typedef boost::function<void (int sockfd, const InetAddress&)> NewConnectionCallback;

		  Acceptor(EventLoop* loop, const InetAddress& listenAddr, bool reuseport);
		  ~Acceptor();

		  void setNewConnectionCallback(const NewConnectionCallback& cb){ _newConnectionCallback = cb; }

		  bool listenning() const { return _listenning; }
		  void listen();

		 private:
		  void handleRead();

		  EventLoop* _loop;
		  Socket _acceptSocket;
		  Handler _acceptHandler;
		  NewConnectionCallback _newConnectionCallback;
		  bool _listenning;
		};

	}
}


#endif /* NET_ACCEPTOR_H_ */
