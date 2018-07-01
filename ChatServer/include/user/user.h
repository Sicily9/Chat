/*
 * user.h
 *
 *  Created on: 2018-5-3
 *      Author: xc
 */
#ifndef CHATSERVER_INCLUDE_USER_USER_H_
#define CHATSERVER_INCLUDE_USER_USER_H_
#include <string>
#include <boost/shared_ptr.hpp>

struct User{
	int _id;
	std::string _passwd;
	std::string _sex;
	std::string _nickname;
	int _head;

	User(int id, std::string passwd, std::string sex, std::string nickname, int head):
		_id(id), _passwd(passwd), _sex(sex), _nickname(nickname), _head(head){

	}
	User(const User& user){
		_id = user._id;
		_passwd = user._passwd;
		_sex = user._sex;
		_nickname = user._nickname;
		_head = user._head;
	}
	User(){}

};



#endif /* CHATSERVER_INCLUDE_USER_USER_H_ */
