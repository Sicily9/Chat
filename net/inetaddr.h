/*
 * inetaddr.h
 *
 *  Created on: 2018年3月13日
 *      Author: xc
 */

#ifndef NET_INETADDR_H_
#define NET_INETADDR_H_

#include <string>
#include <netinet/in.h>

namespace xc{
	namespace net{
		namespace socketsop{
			const struct sockaddr* sockaddr_cast(const struct sockaddr_in6* addr);
		}
		class InetAddress
		{
		 public:

		  explicit InetAddress(uint16_t port = 0, bool loopbackOnly = false, bool ipv6 = false);

		  InetAddress(std::string ip, uint16_t port, bool ipv6 = false);

		  explicit InetAddress(const struct sockaddr_in& addr)
		    : _addr(addr)
		  { }

		  explicit InetAddress(const struct sockaddr_in6& addr)
		    : _addr6(addr)
		  { }

		  sa_family_t family() const { return _addr.sin_family; }
		  std::string toIp() const;
		  std::string toIpPort() const;
		  uint16_t toPort() const;

		  const struct sockaddr* getSockAddr() const { return socketsop::sockaddr_cast(&_addr6); }
		  void setSockAddrInet6(const struct sockaddr_in6& addr6) { _addr6 = addr6; }

		  uint32_t ipNetEndian() const;
		  uint16_t portNetEndian() const { return _addr.sin_port; }

		  static bool resolve(std::string hostname, InetAddress* result);

		 private:
		  union
		  {
		    struct sockaddr_in _addr;
		    struct sockaddr_in6 _addr6;
		  };
		};
	}
}



#endif /* NET_INETADDR_H_ */
