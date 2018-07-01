/*
 * test_tcpserver.cpp
 *
 *  Created on: 2018年3月24日
 *      Author: xc
 */
#include "../net/reactor.h"
#include "../net/inetaddr.h"
#include "../utility/log/log.h"
#include "include/db/db.h"
#include "include/chatServer/chatServer.h"

#include <utility>
#include <stdio.h>
#include <cstring>
#include <unistd.h>

using namespace xc;
using namespace xc::net;
using namespace xc::base;
int numThreads = 0;

int main(int argc, char* argv[])
{
	Initmysql();

	if (argc > 1)
	{
		numThreads = atoi(argv[1]);
	}
	EventLoop loop;
	InetAddress listenAddr("127.0.0.1",2000);
	ChatServer server(&loop, listenAddr);

	server.start();

	loop.loop();
}

