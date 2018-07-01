/*
 * acceptor.c
 *
 *  Created on: 2018年3月13日
 *      Author: xc
 */
#ifndef NET_ACCEPTOR_C_
#define NET_ACCEPTOR_C_

#include "acceptor.h"
#include "reactor.h"
#include "inetaddr.h"
#include "socketop.h"
#include "../utility/log/log.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

using namespace xc;
using namespace xc::net;

Acceptor::Acceptor(EventLoop* loop, const InetAddress& listenAddr, bool reuseport)
  : _loop(loop),
    _acceptSocket(socketsop::createNonblockingOrDie(listenAddr.family())),
    _acceptHandler(loop, _acceptSocket.fd()),
    _listenning(false)
{
  _acceptSocket.setReuseAddr(true);
  _acceptSocket.setReusePort(reuseport);
  _acceptSocket.bindAddress(listenAddr);

  _acceptHandler.setReadCallback([this](){
	  this->handleRead();
  });
}

Acceptor::~Acceptor()
{
	XC_LOG_DEBUG("acceptor quit");
	_acceptHandler.disableAll();
	_acceptHandler.remove();
}

void Acceptor::listen()
{
  _listenning = true;
  _acceptSocket.listen();

  _acceptHandler.enableReading();
  XC_LOG_DEBUG("start listen %d", _acceptSocket.fd());
}

void Acceptor::handleRead()
{
  InetAddress peerAddr;

  int connfd = _acceptSocket.accept(&peerAddr);
  if (connfd >= 0)
  {
    if (_newConnectionCallback)
    {
      _newConnectionCallback(connfd, peerAddr);
    }
    else
    {
      socketsop::close(connfd);
    }
  }

}



#endif /* NET_ACCEPTOR_C_ */


