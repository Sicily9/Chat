/*
 * inetaddr.cpp
 *
 *  Created on: 2018年3月13日
 *      Author: xc
 */
#include "inetaddr.h"
#include "socketop.h"
#include <boost/static_assert.hpp>
#include <netdb.h>
#include <strings.h>  // bzero
#include <netinet/in.h>
#include <endian.h>
#include <cassert>

using namespace xc;
using namespace xc::net;

static const in_addr_t kInaddrAny = INADDR_ANY;
static const in_addr_t kInaddrLoopback = INADDR_LOOPBACK;

BOOST_STATIC_ASSERT(sizeof(InetAddress) == sizeof(struct sockaddr_in6));
BOOST_STATIC_ASSERT(offsetof(sockaddr_in, sin_family) == 0);
BOOST_STATIC_ASSERT(offsetof(sockaddr_in6, sin6_family) == 0);
BOOST_STATIC_ASSERT(offsetof(sockaddr_in, sin_port) == 2);
BOOST_STATIC_ASSERT(offsetof(sockaddr_in6, sin6_port) == 2);


InetAddress::InetAddress(uint16_t port, bool loopbackOnly, bool ipv6)
{
  BOOST_STATIC_ASSERT(offsetof(InetAddress, _addr6) == 0);
  BOOST_STATIC_ASSERT(offsetof(InetAddress, _addr) == 0);
  if (ipv6)
  {
    bzero(&_addr6, sizeof _addr6);
    _addr6.sin6_family = AF_INET6;
    in6_addr ip = loopbackOnly ? in6addr_loopback : in6addr_any;
    _addr6.sin6_addr = ip;
    _addr6.sin6_port = htobe16(port);
  }
  else
  {
    bzero(&_addr, sizeof _addr);
    _addr.sin_family = AF_INET;
    in_addr_t ip = loopbackOnly ? kInaddrLoopback : kInaddrAny;
    _addr.sin_addr.s_addr = htobe32(ip);
    _addr.sin_port = htobe16(port);
  }
}

InetAddress::InetAddress(std::string ip, uint16_t port, bool ipv6)
{
  if (ipv6)
  {
    bzero(&_addr6, sizeof _addr6);
    socketsop::fromIpPort(ip.c_str(), port, &_addr6);
  }
  else
  {
    bzero(&_addr, sizeof _addr);
    socketsop::fromIpPort(ip.c_str(), port, &_addr);
  }
}

std::string InetAddress::toIpPort() const
{
  char buf[64] = "";
  socketsop::toIpPort(buf, sizeof buf, getSockAddr());
  return buf;
}

std::string InetAddress::toIp() const
{
  char buf[64] = "";
  socketsop::toIp(buf, sizeof buf, getSockAddr());
  return buf;
}

uint32_t InetAddress::ipNetEndian() const
{
  assert(family() == AF_INET);
  return _addr.sin_addr.s_addr;
}

uint16_t InetAddress::toPort() const
{
  return be16toh(portNetEndian());
}

static char t_resolveBuffer[64 * 1024];

bool InetAddress::resolve(std::string hostname, InetAddress* out)
{
  assert(out != NULL);
  struct hostent hent;
  struct hostent* he = NULL;
  int herrno = 0;
  bzero(&hent, sizeof(hent));

  int ret = gethostbyname_r(hostname.c_str(), &hent, t_resolveBuffer, sizeof t_resolveBuffer, &he, &herrno);
  if (ret == 0 && he != NULL)
  {
    assert(he->h_addrtype == AF_INET && he->h_length == sizeof(uint32_t));
    out->_addr.sin_addr = *reinterpret_cast<struct in_addr*>(he->h_addr);
    return true;
  }
  else
  {
    if (ret)
    {

    }
    return false;
  }
}

