/*
 * db.h
 *
 *  Created on: 2018-5-2
 *      Author: xc
 */
#ifndef CHATSERVER_INCLUDE_DB_DB_H_
#define CHATSERVER_INCLUDE_DB_DB_H_
#include <mysql/mysql.h>
#include <boost/shared_ptr.hpp>
#include "../proto/TcpProto.pb.h"
#include "../user/user.h"

extern MYSQL mysql;

void Initmysql();
int createNumber(int id);
int registerUserForID(const boost::shared_ptr<RegistReq>& concreteMsg, int &res);
User checkUserInfo(const boost::shared_ptr<LoginReq>& concreteMsg, int& err);
User queryUserFromID(int id, int &err);


#endif /* CHATSERVER_INCLUDE_DB_DB_H_ */
