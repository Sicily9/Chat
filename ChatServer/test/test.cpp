/*
 * test.cpp
 *
 *  Created on: 2018-5-6
 *      Author: xc
 */
#include "../include/codec/codec.h"
#include "TestProto.pb.h"
#include <cstdlib>
#include <string>

int main(){
	test t;
	t.set_code("111112");
	std::string buf = encode(t);

	int32_t be32 = 0;
	std::copy(buf.begin(), buf.begin() + sizeof be32, reinterpret_cast<char*>(&be32));
	int32_t len = ::ntohl(be32);

	be32 = 0;
	std::copy(buf.begin() + sizeof be32, buf.begin() + 2*sizeof be32, reinterpret_cast<char*>(&be32));
	int32_t nameLen = ::ntohl(be32);
	std::cout<<nameLen<<std::endl;

	std::string s;
	s.resize(nameLen+1);
	std::copy(buf.begin() + 2*sizeof be32, buf.begin() + 2*sizeof be32 + nameLen, s.begin());
	std::cout<<s<<std::endl;

	std::string transport = buf.substr(sizeof(int32_t));
	google::protobuf::Message* msg= decode(transport);
	std::cout<<static_cast<test *>(msg)->code()<<std::endl;

}

