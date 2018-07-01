/*
 * socket.h
 *
 *  Created on: 2018年3月13日
 *      Author: xc
 */

#ifndef NET_SOCKET_H_
#define NET_SOCKET_H_

#include <boost/noncopyable.hpp>

namespace xc{
	namespace net{
		class InetAddress;
		class Socket :public boost::noncopyable{
		public:
			explicit Socket(int fd):
			_sockfd(fd){

			}

			~Socket();
			int fd() const { return _sockfd; }

			void bindAddress(const InetAddress& localaddr);

			void listen();

			int accept(InetAddress* peeraddr);

			void shutdownWrite();

			void setTcpNoDelay(bool on);

			void setReuseAddr(bool on);

			void setReusePort(bool on);

			void setKeepAlive(bool on);
		private:
			const int _sockfd;
		};
	}
}




#endif /* NET_SOCKET_H_ */
