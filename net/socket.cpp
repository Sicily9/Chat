/*
 * socket.cpp
 *
 *  Created on: 2018年3月13日
 *      Author: xc
 */

#include "socket.h"
#include "inetaddr.h"
#include "socketop.h"
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <strings.h>  // bzero
#include <stdio.h>  // snprintf

using namespace xc;
using namespace xc::net;

Socket::~Socket()
{
  socketsop::close(_sockfd);
}

void Socket::bindAddress(const InetAddress& addr)
{
	socketsop::bindOrDie(_sockfd, addr.getSockAddr());
}

void Socket::listen()
{
  socketsop::listenOrDie(_sockfd);
}

int Socket::accept(InetAddress* peeraddr)
{
	struct sockaddr_in6 addr;
	bzero(&addr, sizeof addr);
	int connfd = socketsop::accept(_sockfd, &addr);
	if (connfd >= 0)
	{
		peeraddr->setSockAddrInet6(addr);
	}
	return connfd;
}

void Socket::shutdownWrite()
{
	socketsop::shutdownWrite(_sockfd);
}

void Socket::setTcpNoDelay(bool on)
{
  int optval = on ? 1 : 0;
  ::setsockopt(_sockfd, IPPROTO_TCP, TCP_NODELAY,
               &optval, static_cast<socklen_t>(sizeof optval));

}

void Socket::setReuseAddr(bool on)
{
  int optval = on ? 1 : 0;
  ::setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR,
               &optval, static_cast<socklen_t>(sizeof optval));

}

void Socket::setReusePort(bool on)
{
  int optval = on ? 1 : 0;
  int ret = ::setsockopt(_sockfd, SOL_SOCKET, SO_REUSEPORT,
		  &optval, static_cast<socklen_t>(sizeof optval));
  if (ret < 0 && on)
  {

  }
}

void Socket::setKeepAlive(bool on)
{
  int optval = on ? 1 : 0;
  ::setsockopt(_sockfd, SOL_SOCKET, SO_KEEPALIVE,
		  &optval, static_cast<socklen_t>(sizeof optval));
}

