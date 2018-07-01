#include "../include/room/room.h"
#include "../include/room/room_internal.h"
#include "../include/db/db.h"
#include "../../utility/log/log.h"
#include "../include/codec/codec.h"
#include <boost/shared_ptr.hpp>
#include <cstdio>
#include <cstdlib>
#include <ctime>

Room * Room::_room = nullptr;
std::mutex _mutex;

Room::Room(): _hc(0) {

}

std::map<int, TcpConnectionPtr>& Room::getUserConnMap()
{
	return _userConnMap;
}

std::map<int, User>& Room::getUserMap()
{
	return _userMap;
}

Room* Room::GetInstance()
{
	if(!_room)
		_room = new Room();

	return _room;
}

void Room::decreaseNumber()
{
	_hc--;
}

void Room::addNumber()
{
	_hc++;
}

void registerUserInfo(const TransportDataPtr& data)
{
	boost::shared_ptr<RegistReq> concreteMsg =  boost::static_pointer_cast<RegistReq>(data->_msgData);
	int res = 0;
	int id = registerUserForID(concreteMsg, res);

	if(res){
		XC_LOG_DEBUG("register failed");
		return;
	}else{
		RegistRes response;
		response.set_code(1);
		response.set_id(createNumber(id));
		std::string transport = encode(response);
		data->_tcpConn->send(transport);
		XC_LOG_DEBUG("register success");
	}
}

void userJoinRoom(const TransportDataPtr& data)
{

	Room* room =Room::GetInstance();
	std::map<int, User>& userMap = room->getUserMap();

	boost::shared_ptr<LoginReq> concreteMsg =  boost::static_pointer_cast<LoginReq>(data->_msgData);
	int err = 1;

	User user = checkUserInfo(concreteMsg, err);

	if(err == -1){
		XC_LOG_ERROR("failed");
		return;
	}

	{
		std::lock_guard<std::mutex> locker(_mutex);
		room->addNumber();
		room->getUserConnMap()[concreteMsg->id()] = data->_tcpConn;
		userMap[concreteMsg->id()] = user;
	}

	std::string transport = EncodeMsgLoginResToSelf(user);
	std::string transport2 = EncodeMsgLoginResToOther(user);

	for(auto &it : room->getUserConnMap()){
		if(it.first == user._id){
			it.second->send(transport);
		} else {
			it.second->send(transport2);
		}
	}
	XC_LOG_DEBUG("join success");
}

void userExitRoom(const TransportDataPtr& data)
{
	boost::shared_ptr<LogoutReq> concreteMsg =  boost::static_pointer_cast<LogoutReq>(data->_msgData);
	Room * room =Room::GetInstance();
	int id = concreteMsg->id();
	int err = 1;

	User user = queryUserFromID(id, err);

	if(err == -1){
		XC_LOG_ERROR("failed");
		return;
	}

	LogoutRes response;
	Person *person = new Person;
	response.set_code(1);
	setPerson(response, person, user);
	std::string transport = encode(response);

	for(auto it : room->getUserConnMap()){
		if(it.first == user._id){
			it.second->shutdown();
			XC_LOG_DEBUG("%d shutdown", it.first);
		}else{
			it.second->send(transport);
		}
	}

	std::lock_guard<std::mutex> locker(_mutex);
	room->decreaseNumber();
	room->getUserConnMap().erase(concreteMsg->id());
	room->getUserMap().erase(concreteMsg->id());
	XC_LOG_DEBUG("%d start quit3", id);
}

void castGroupMsg(const TransportDataPtr& data)
{
	boost::shared_ptr<TextMsgGroup> concreteMsg =  boost::static_pointer_cast<TextMsgGroup>(data->_msgData);
	std::string transport = encode(*concreteMsg);

	Room * room =Room::GetInstance();
	for(auto &it : room->getUserConnMap()){
		if(it.first != concreteMsg->src()){
			it.second->send(transport);
		}
	}

}

void castPrivateMsg(const TransportDataPtr& data)
{
	boost::shared_ptr<TextMsgPrivate> concreteMsg =  boost::static_pointer_cast<TextMsgPrivate>(data->_msgData);
	std::string transport = encode(*concreteMsg);

	Room * room =Room::GetInstance();
	for(auto &it : room->getUserConnMap()){
		if(it.first == concreteMsg->dst()){
			it.second->send(transport);
		}
	}
}

void castShakeMsg(const TransportDataPtr& data){
	boost::shared_ptr<ShakeMsg> concreteMsg =  boost::static_pointer_cast<ShakeMsg>(data->_msgData);
	std::string transport = encode(*concreteMsg);

	Room * room =Room::GetInstance();
	for(auto &it : room->getUserConnMap()){
		if(it.first == concreteMsg->dst()){
			it.second->send(transport);
		}
	}
}
