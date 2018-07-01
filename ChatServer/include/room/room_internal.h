/*
 * room_internal.h
 *
 *  Created on: 2018-5-16
 *      Author: xc
 */

#ifndef CHATSERVER_INCLUDE_ROOM_ROOM_INTERNAL_H_
#define CHATSERVER_INCLUDE_ROOM_ROOM_INTERNAL_H_
#include "room.h"

void setPerson( LoginRes &response, Person *person, User &user);
void setPerson( LogoutRes &response, Person *person, User &user);
void setOtherOnlinePerson(LoginRes &response, Person *person, User &user);
std::string EncodeMsgLoginResToSelf(User &user);
std::string EncodeMsgLoginResToOther(User &user);




#endif /* CHATSERVER_INCLUDE_ROOM_ROOM_INTERNAL_H_ */
