#include "../include/db/db.h"
#include "../../utility/log/log.h"

MYSQL mysql;

void Initmysql()
{
	mysql_init(&mysql);
	mysql_real_connect(&mysql, "localhost", "root", "960829", "test", 3306, NULL, 0);
}

int createNumber(int id)
{
	bool flag = true;
	int number;
	while(1){
		srand((unsigned)time(NULL));
		number = rand()%100000;
		char s[256];
		sprintf(s, "select user_id from user where user_id = %d ", number);
		int res=mysql_query(&mysql,s);

		MYSQL_RES *result = mysql_store_result(&mysql);
		int rows = mysql_num_rows(result);

		if(!res){
			if(rows == 0){
				flag = false;
				sprintf(s, "update user set user_id = %d where id = %d ", number, id);
				mysql_query(&mysql,s);
				break;
			}
		}

	}

	return number;
}

int registerUserForID(const boost::shared_ptr<RegistReq>& concreteMsg, int &res)
{
	char s[256];
	sprintf(s, "insert into user(user_nickname, user_sex, user_password, user_head) values('%s', '%s', '%s', '%d')",
			concreteMsg->nickname().c_str(), concreteMsg->sex().c_str(),
			concreteMsg->password().c_str(), concreteMsg->head()
		 );
	std::string cmd(s);
	res=mysql_real_query(&mysql, cmd.c_str(), cmd.length());
	int id = mysql_insert_id(&mysql);

	return id;
}

User checkUserInfo(const boost::shared_ptr<LoginReq>& concreteMsg, int& err)
{
	char s[256];
	sprintf(s, "select * from user where user_id = %d and user_password = '%s' ", concreteMsg->id(), concreteMsg->password().c_str());
	int result=mysql_query(&mysql,s);
	if(result){
		err = -1;
	}
	MYSQL_RES *res = mysql_store_result(&mysql);
	if(res == nullptr){
		err = -1;
	}
	MYSQL_ROW row = mysql_fetch_row(res);
	User user(strtol(row[1], NULL, 10), row[4], row[5], row[3], strtol(row[2], NULL, 10));
	return user;
}

User queryUserFromID(int id, int &err)
{
	char s[256];
	sprintf(s, "select * from user where user_id = %d", id);
	int result=mysql_query(&mysql,s);
	if(result){
		err = -1;
	}

	MYSQL_RES *res = mysql_store_result(&mysql);
	int rows = mysql_num_rows(res);
	MYSQL_ROW row = mysql_fetch_row(res);
	User user(strtol(row[1], NULL, 10), row[4], row[5], row[3], strtol(row[2], NULL, 10));
	return user;
}

