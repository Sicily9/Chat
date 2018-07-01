/*
 * test.cpp
 *
 *  Created on: 2018-6-7
 *      Author: xc
 */

#include <string>
#include <iostream>
#include <cstring>

class BigInteger{
public:
	BigInteger();
	BigInteger(std::string data);
	char* getData();
	BigInteger(const BigInteger& temp);
	BigInteger& operator= (const BigInteger& temp);
	BigInteger operator+ (const BigInteger& temp);
private:
	char* _data;
};

char *  BigInteger::getData(){
	return _data;
}

BigInteger::BigInteger(std::string data){
		_data = new char[data.size()];
		int i = data.size() - 1;
		for(; i > 0; i--){
			_data[i] = data[i];
		}
		data[i] = '0';
	}

BigInteger& BigInteger::operator= (const BigInteger& temp)
{
		char* t = temp.getData();
		_data = new char[strlen(t)];
		for(int i =0; i< strlen(t); i++){
			_data[i] = t[i];
		}
}

BigInteger::BigInteger(const BigInteger& temp){
	char* t = temp.getData();
	_data = new char[strlen(t)];
	for(int i =0; i< strlen(t); i++){
		_data[i] = t[i];
	}
}

BigInteger::BigInteger()
{
	_data = new char;
	*_data ='0' ;
}

std::ostream & operator<<(std::ostream &out, BigInteger &obj){
	char * t =obj.getData();
	for(int i = strlen(t) - 1; i >0 ; i --){
		std::cout<<t[i];
	}
}

BigInteger BigInteger::operator+ (const BigInteger &temp){
	int i = atoi(_data);
	int i2 = atoi(temp.getData());
	BigInteger temp(std::to_string(i+i2));
	return temp;

}
