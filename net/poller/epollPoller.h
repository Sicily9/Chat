/*
 * EpollPoller.h
 *
 *  Created on: 2018年3月8日
 *      Author: xc
 */

#ifndef NET_POLLER_EPOLLPOLLER_H_
#define NET_POLLER_EPOLLPOLLER_H_

#include "basePoller.h"
#include <vector>

struct epoll_event;

namespace xc{
	namespace net{
		class EpollPoller : public Poller
		{
		public:
			EpollPoller();
			virtual ~EpollPoller();

			int poll(int timeoutMs, HandlerList* activeChannels);
			void updateHandler(Handler* handler);
			void removeHandler(Handler* handlerl);
			bool hasHandler(Handler* handle) const;

		private:
			static const int kInitEventListSize = 16;
			static const char* operationToString(int op);
			void fillActiveChannels(int numEvents,HandlerList* activeHandlers) const;
			void update(int operation, Handler* handler);

			typedef std::map<int, Handler*> HandlerMap;
			HandlerMap _handlers;

			typedef std::vector<struct epoll_event> EventList;
			int _epollfd;
			EventList _events;
		};
	}
}


#endif /* NET_POLLER_EPOLLPOLLER_H_ */
