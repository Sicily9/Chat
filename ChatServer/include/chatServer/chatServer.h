/*
 * chatServer.h
 *
 *  Created on: 2018-5-1
 *      Author: xc
 */
#ifndef NET_TEST_UNIT_CHATSERVER_INCLUDE_CHATSERVER_CHATSERVER_H_
#define NET_TEST_UNIT_CHATSERVER_INCLUDE_CHATSERVER_CHATSERVER_H_

#include "../../../net/tcpserver.h"
#include "../../../net/reactor.h"
#include "../../../utility/msghandler/msghandler.h"
#include <mutex>

using namespace xc;
using namespace xc::net;
using namespace xc::base;


class ChatServer
{
public:
	ChatServer(EventLoop* loop, const InetAddress& listenAddr);
	void start();

private:
	void onConnection(const TcpConnectionPtr& conn);

	void onMessage(const TcpConnectionPtr& conn, Buffer* buf);

	void registerAllMessage();

	std::mutex  _mutex;
	EventLoop* _loop;
	TcpServer _server;
	MessageHandler _msgHandler;
};




#endif /* NET_TEST_UNIT_CHATSERVER_INCLUDE_CHATSERVER_CHATSERVER_H_ */
