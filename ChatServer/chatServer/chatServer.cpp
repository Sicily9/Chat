/*
 * chatServer.cpp
 *
 *  Created on: 2018-5-1
 *      Author: xc
 */
#include "../include/chatServer/chatServer.h"
#include "../include/codec/codec.h"
#include "../include/room/room.h"
#include "../include/proto/TcpProto.pb.h"
#include "../../utility/log/log.h"
#include <boost/shared_ptr.hpp>
#include <cstdlib>
#define NUMBER  3

using namespace xc::base;
ChatServer::ChatServer(EventLoop* loop, const InetAddress& listenAddr):
_loop(loop), _server(loop, listenAddr, "EchoServer"), _msgHandler(10)
{
	_server.setConnectionCallback([this](const TcpConnectionPtr& conn){
		this->onConnection(conn);
	});
	_server.setMessageCallback([this](const TcpConnectionPtr& conn, Buffer* buf) {
		this->onMessage(conn, buf);
	});
	_server.setThreadNum(NUMBER);
	registerAllMessage();
	_msgHandler.Run();
}

void ChatServer::start()
{
	_server.start();
}

void ChatServer::onConnection(const TcpConnectionPtr& conn)
{

}

void ChatServer::onMessage(const TcpConnectionPtr& conn, Buffer* buf)
{
	std::string msgString(buf->retrieveAllAsString());

	//检查消息头部 随便构造一个消息发过来 是不会接收的
	int32_t be32 = 0;
	std::copy(msgString.begin() , msgString.begin()  + sizeof be32, reinterpret_cast<char*>(&be32));
	int32_t len = ::ntohl(be32);
	assert(len == msgString.size() - sizeof(be32));

	MessagePtr  msg(decode(msgString.substr(sizeof(int32_t))));

	TransportDataPtr TransportDataPtr(new TransportData(msg, conn));

	_mutex.lock();
	_msgHandler.push(TransportDataPtr);
	_mutex.unlock();

}

void ChatServer::registerAllMessage()
{
	_msgHandler.getDispatcher().registerMessageCallBack(LoginReq::descriptor(), userJoinRoom);
	_msgHandler.getDispatcher().registerMessageCallBack(RegistReq::descriptor(), registerUserInfo);
	_msgHandler.getDispatcher().registerMessageCallBack(LogoutReq::descriptor(), userExitRoom);
	_msgHandler.getDispatcher().registerMessageCallBack(TextMsgPrivate::descriptor(), castPrivateMsg);
	_msgHandler.getDispatcher().registerMessageCallBack(TextMsgGroup::descriptor(), castGroupMsg);
	_msgHandler.getDispatcher().registerMessageCallBack(ShakeMsg::descriptor(), castShakeMsg);
}
