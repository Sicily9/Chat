/*
 * reactor.h
 *
 *  Created on: 2018年2月27日
 *      Author: xc
 */
#ifndef NET_REACTOR_H_
#define NET_REACTOR_H_
#include <atomic>
#include <thread>
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/function.hpp>
#include <mutex>

namespace xc{
	namespace net{
		class Poller;
		class Handler;

		class EventLoop : public boost::noncopyable {
		public:
			typedef boost::function<void()> Functor;
			EventLoop();
			~EventLoop();
			void loop();
			void quit();
			void updateHandler(Handler *handler);
			void removeHandler(Handler *handler);
			bool hasHandler(Handler* handler);
			bool isInLoopThread();
			static void configInit();
			void wakeup();
			void handleRead();
			void runInLoop(const Functor& cb);
			void queueInLoop(const Functor& cb);
			void doPendingFunctors();
		private:
			static bool _inited;
			typedef std::vector<Handler*> HandlerList;

			boost::scoped_ptr<Poller> _poller;
			std::atomic_bool _looping;
			std::atomic_bool _quit;
			const pthread_t _tid;
			int _wakeupFd;
			std::atomic_bool _callingPendingFunctors;
			boost::scoped_ptr<Handler> _wakeupHandler;
			HandlerList _activeHandlers;
			Handler* _currentActiveHandler;
			std::vector<Functor> _pendingFunctors;
			std::mutex _mutex;
		};
	}
}



#endif /* NET_REACTOR_H_ */
