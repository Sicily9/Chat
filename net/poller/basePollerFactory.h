/*
 * basePollerFactory.h
 *
 *  Created on: 2018年3月18日
 *      Author: xc
 */
#ifndef NET_POLLER_BASEPOLLERFACTORY_H_
#define NET_POLLER_BASEPOLLERFACTORY_H_
#include <boost/noncopyable.hpp>

namespace xc{
	namespace net{
		class Poller;
		class PollerFactory : public boost::noncopyable
		{
		public:
			enum Type {
				EPOLL ,
				POLL ,
				SELECT
			};
			PollerFactory();
			~PollerFactory();
			static Poller* CreatePoller(Type id);
		};

	}
}

#endif /* NET_POLLER_BASEPOLLERFACTORY_H_ */



