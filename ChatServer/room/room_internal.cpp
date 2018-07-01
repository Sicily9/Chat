/*
 * room_internal.cpp
 *
 *  Created on: 2018-5-16
 *      Author: xc
 */

#include "../include/room/room_internal.h"
#include "../../utility/log/log.h"
#include "../include/codec/codec.h"

void setPerson( LoginRes &response, Person *person, User &user)
{
	person = response.add_person();
	person->set_id(user._id);
	person->set_nickname(user._nickname);
	person->set_password(user._passwd);
	person->set_sex(user._sex);
	person->set_head(user._head);
}

void setPerson( LogoutRes &response, Person *person, User &user)
{
	person->set_id(user._id);
	person->set_nickname(user._nickname);
	person->set_password(user._passwd);
	person->set_sex(user._sex);
	person->set_head(user._head);
	response.set_allocated_person(person);
}

void setOtherOnlinePerson(LoginRes &response, Person *person, User &user)
{
	Room * room =Room::GetInstance();
	for(auto &it : room->getUserMap()){
		if(user._id != it.first){
			person = response.add_person();
			person->set_id(it.second._id);
			person->set_nickname(it.second._nickname);
			person->set_password(it.second._passwd);
			person->set_sex(it.second._sex);
			person->set_head(it.second._head);
		}
	}
}

std::string EncodeMsgLoginResToSelf(User &user)
{
	LoginRes response;
	Person *person;
	response.set_code(1);
	setPerson(response, person, user);
	setOtherOnlinePerson(response, person, user);
	std::string transport = encode(response);

	return transport;
}

std::string EncodeMsgLoginResToOther(User &user){
	LoginRes response;
	Person *person;
	response.set_code(1);
	setPerson(response, person, user);
	std::string transport = encode(response);

	return transport;
}
