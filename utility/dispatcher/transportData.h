/*
 * transportData.h
 *
 *  Created on: 2018-5-1
 *      Author: xc
 */
#ifndef UTILITY_DISPATCHER_TRANSPORTDATA_H_
#define UTILITY_DISPATCHER_TRANSPORTDATA_H_

#include <google/protobuf/message.h>
#include "../../net/tcpconnection.h"
using namespace xc::net;


typedef boost::shared_ptr<google::protobuf::Message> MessagePtr;

struct TransportData :public boost::noncopyable{
	MessagePtr _msgData;
	TcpConnectionPtr _tcpConn;
	TransportData(MessagePtr msgData, TcpConnectionPtr tcpConn): _msgData(msgData), _tcpConn(tcpConn){}
};
typedef boost::shared_ptr<TransportData> TransportDataPtr;

#endif /* UTILITY_MSGHANDLER_TRANSPORTDATA_H_ */
