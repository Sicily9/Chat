/*
 * room.h
 *
 *  Created on: 2018-5-1
 *      Author: xc
 */
#ifndef NET_TEST_UNIT_CHATSERVER_ROOM_ROOM_H_
#define NET_TEST_UNIT_CHATSERVER_ROOM_ROOM_H_
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include "../../../utility/msghandler/msghandler.h"
#include "../../../utility/dispatcher/transportData.h"
#include "../proto/TcpProto.pb.h"
#include "../user/user.h"
#include <atomic>
#include <mutex>
#include <map>

using namespace xc::base;

class Room : public boost::noncopyable{
public:
	static Room* GetInstance();
	std::map<int, TcpConnectionPtr>& getUserConnMap();
	std::map<int, User>& getUserMap();
	void addNumber();
	void decreaseNumber();
	 ~Room() {}
private:
	explicit Room();

	std::map<int, TcpConnectionPtr > _userConnMap;
	std::map<int, User> _userMap;
	static Room* _room;
	std::atomic_int _hc;
};

void registerUserInfo(const TransportDataPtr& data);
void userJoinRoom(const TransportDataPtr& data);
void userExitRoom(const TransportDataPtr& data);
void castPrivateMsg(const TransportDataPtr& data);
void castGroupMsg(const TransportDataPtr& data);
void castShakeMsg(const TransportDataPtr& data);


#endif /* NET_TEST_UNIT_CHATSERVER_ROOM_ROOM_H_ */
