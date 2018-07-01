/*
 * Poller.h
 *
 *  Created on: 2018年3月1日
 *      Author: xc
 */

#ifndef NET_POLLER_H_
#define NET_POLLER_H_

#include "../reactor.h"
#include<boost/noncopyable.hpp>
#include<map>
#include<vector>
#include<string>

namespace xc{
	namespace net{

		class Handler;
		class Poller : public boost::noncopyable
		{
		public:
			typedef std::vector<Handler*> HandlerList;

			Poller(){

			}
			virtual ~Poller(){

			}

			virtual int poll(int timeoutMs, HandlerList* activeHandlers) = 0;

			virtual void updateHandler(Handler* handle) = 0;

			virtual void removeHandler(Handler* handle) = 0;

			virtual bool hasHandler(Handler* handle) const = 0;
		};

	}
}




#endif /* NET_POLLER_H_ */
