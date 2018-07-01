/*
 * basePollerFactory.cpp
 *
 *  Created on: 2018年3月18日
 *      Author: xc
 */
#include "basePollerFactory.h"
#include "epollPoller.h"

using namespace xc;
using namespace xc::net;

PollerFactory::PollerFactory()
{

}

Poller* PollerFactory::CreatePoller(Type id)
{
	Poller* poller = nullptr;
	switch(id){
	case PollerFactory::EPOLL:
		poller = new EpollPoller();
		break;
	case PollerFactory::POLL:
		poller = nullptr;
		break;
	case PollerFactory::SELECT:
		poller = nullptr;
		break;
	default:
		break;
	}
	return poller;
}

PollerFactory::~PollerFactory()
{

}

