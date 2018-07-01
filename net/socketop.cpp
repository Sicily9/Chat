/*
 * socketop.cpp
 *
 *  Created on: 2018年3月15日
 *      Author: xc
 */
#include "socketop.h"
#include "../utility/log/log.h"
#include <errno.h>
#include <fcntl.h>
#include <cstdio>  // snprintf
#include <cstring>  // bzero
#include <sys/socket.h>
#include <sys/uio.h>  // readv
#include <unistd.h>
#include <cassert>
#include<boost/implicit_cast.hpp>

using namespace xc;
using namespace xc::net;

const struct sockaddr* socketsop::sockaddr_cast(const struct sockaddr_in6* addr)
{
  return static_cast<const struct sockaddr*>(boost::implicit_cast<const void*>(addr));
}

struct sockaddr* socketsop::sockaddr_cast(struct sockaddr_in6* addr)
{
  return static_cast<struct sockaddr*>(boost::implicit_cast<void*>(addr));
}

const struct sockaddr* socketsop::sockaddr_cast(const struct sockaddr_in* addr)
{
	return static_cast<const struct sockaddr*>(boost::implicit_cast<const void*>(addr));
}

const struct sockaddr_in* socketsop::sockaddr_in_cast(const struct sockaddr* addr)
{
	return static_cast<const struct sockaddr_in*>(boost::implicit_cast<const void*>(addr));
}

const struct sockaddr_in6* socketsop::sockaddr_in6_cast(const struct sockaddr* addr)
{
	return static_cast<const struct sockaddr_in6*>(boost::implicit_cast<const void*>(addr));
}

int socketsop::createNonblockingOrDie(sa_family_t family)
{

  int sockfd = ::socket(family, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
  if (sockfd < 0)
  {
	  XC_LOG_ERROR("create sockfd failed");
	  exit(-1);
  }

  return sockfd;
}



void socketsop::bindOrDie(int sockfd, const struct sockaddr* addr)
{
	int ret = ::bind(sockfd, addr, static_cast<socklen_t>(sizeof(struct sockaddr_in6)));
	if (ret < 0){
		XC_LOG_ERROR("bind failed");
		exit(-1);
	}
}

void socketsop::listenOrDie(int sockfd)
{
	int ret = ::listen(sockfd, SOMAXCONN);
    if (ret < 0){
    	XC_LOG_ERROR("listen failed");
	    exit(-1);
    }
}

int socketsop::accept(int sockfd, struct sockaddr_in6* addr)
{
	socklen_t addrlen = static_cast<socklen_t>(sizeof *addr);

	int connfd = ::accept4(sockfd, sockaddr_cast(addr),
			&addrlen, SOCK_NONBLOCK | SOCK_CLOEXEC);

	if (connfd < 0) {
		int savedErrno = errno;

		switch (savedErrno)
		{
			case EAGAIN:
			case ECONNABORTED:
			case EINTR:
			case EPROTO: // ???
			case EPERM:
			case EMFILE: // per-process lmit of open file desctiptor ???
				// expected errors
				errno = savedErrno;
				break;
			case EBADF:
			case EFAULT:
			case EINVAL:
			case ENFILE:
			case ENOBUFS:
			case ENOMEM:
			case ENOTSOCK:
			case EOPNOTSUPP:
				// unexpected errors
				break;
			default:
				break;
		}
	}
	return connfd;
}

int socketsop::connect(int sockfd, const struct sockaddr* addr)
{
	return ::connect(sockfd, addr, static_cast<socklen_t>(sizeof(struct sockaddr_in6)));
}

ssize_t socketsop::read(int sockfd, void *buf, size_t count)
{
	return ::read(sockfd, buf, count);
}

ssize_t socketsop::readv(int sockfd, const struct iovec *iov, int iovcnt)
{
	return ::readv(sockfd, iov, iovcnt);
}

ssize_t socketsop::write(int sockfd, const void *buf, size_t count)
{
	return ::write(sockfd, buf, count);
}

void socketsop::close(int sockfd)
{
	if (::close(sockfd) < 0)
	{
	  XC_LOG_ERROR("sockets::close");
	}
}

void socketsop::shutdownWrite(int sockfd)
{
	if (::shutdown(sockfd, SHUT_WR) < 0)
	{

	}
}

void socketsop::toIpPort(char* buf, size_t size,
                       const struct sockaddr* addr)
{
	toIp(buf,size, addr);
	size_t end = strlen(buf);
	const struct sockaddr_in* addr4 = sockaddr_in_cast(addr);
	uint16_t port = be16toh(addr4->sin_port);
	assert(size > end);
	snprintf(buf+end, size-end, ":%u", port);
}

void socketsop::toIp(char* buf, size_t size,
                   const struct sockaddr* addr)
{
	if (addr->sa_family == AF_INET)
	{
		assert(size >= INET_ADDRSTRLEN);
		const struct sockaddr_in* addr4 = sockaddr_in_cast(addr);
		::inet_ntop(AF_INET, &addr4->sin_addr, buf, static_cast<socklen_t>(size));
	}
	else if (addr->sa_family == AF_INET6)
	{
		assert(size >= INET6_ADDRSTRLEN);
		const struct sockaddr_in6* addr6 = sockaddr_in6_cast(addr);
		::inet_ntop(AF_INET6, &addr6->sin6_addr, buf, static_cast<socklen_t>(size));
	}
}

void socketsop::fromIpPort(const char* ip, uint16_t port,
                         struct sockaddr_in* addr)
{
	addr->sin_family = AF_INET;
	addr->sin_port = htobe16(port);
	if (::inet_pton(AF_INET, ip, &addr->sin_addr) <= 0)
	{

	}
}

void socketsop::fromIpPort(const char* ip, uint16_t port,
                         struct sockaddr_in6* addr)
{
	addr->sin6_family = AF_INET6;
	addr->sin6_port = htobe16(port);
	if (::inet_pton(AF_INET6, ip, &addr->sin6_addr) <= 0)
	{

	}
}

int socketsop::getSocketError(int sockfd)
{
	int optval;
	socklen_t optlen = static_cast<socklen_t>(sizeof optval);

	if (::getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &optval, &optlen) < 0)
	{
		return errno;
	}
	else
	{
		return optval;
	}
}

struct sockaddr_in6 socketsop::getLocalAddr(int sockfd)
{
	struct sockaddr_in6 localaddr;
	bzero(&localaddr, sizeof localaddr);
	socklen_t addrlen = static_cast<socklen_t>(sizeof localaddr);
	if (::getsockname(sockfd, sockaddr_cast(&localaddr), &addrlen) < 0)
	{

	}
	return localaddr;
}

struct sockaddr_in6 socketsop::getPeerAddr(int sockfd)
{
	struct sockaddr_in6 peeraddr;
	bzero(&peeraddr, sizeof peeraddr);
	socklen_t addrlen = static_cast<socklen_t>(sizeof peeraddr);
	if (::getpeername(sockfd, sockaddr_cast(&peeraddr), &addrlen) < 0)
	{

	}
	return peeraddr;
}

bool socketsop::isSelfConnect(int sockfd)
{
	struct sockaddr_in6 localaddr = getLocalAddr(sockfd);
	struct sockaddr_in6 peeraddr = getPeerAddr(sockfd);
	if (localaddr.sin6_family == AF_INET)
	{
		const struct sockaddr_in* laddr4 = reinterpret_cast<struct sockaddr_in*>(&localaddr);
		const struct sockaddr_in* raddr4 = reinterpret_cast<struct sockaddr_in*>(&peeraddr);
		return laddr4->sin_port == raddr4->sin_port
				&& laddr4->sin_addr.s_addr == raddr4->sin_addr.s_addr;
	}
	else if (localaddr.sin6_family == AF_INET6)
	{
		return localaddr.sin6_port == peeraddr.sin6_port
				&& memcmp(&localaddr.sin6_addr, &peeraddr.sin6_addr, sizeof localaddr.sin6_addr) == 0;
	}
	else
	{
		return false;
	}
}






