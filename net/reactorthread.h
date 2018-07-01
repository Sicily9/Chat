/*
 * EventLoopThread.h
 *
 *  Created on: 2018年3月20日
 *      Author: xc
 */

#ifndef NET_REACTORTHREAD_H_
#define NET_REACTORTHREAD_H_
#include <atomic>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <mutex>
#include <condition_variable>
#include <thread>
namespace xc{
	namespace net{
		class EventLoop;
		typedef boost::function<void ()> ThreadFunc;
		typedef boost::function<void(EventLoop*)> ThreadInitCallback;
		class EventLoopThread : public boost::noncopyable{
		public:
			 EventLoopThread(const ThreadInitCallback& cb = ThreadInitCallback());
			 ~EventLoopThread();
			 EventLoop* startLoop();
			 void Stop();
			 void Start();
		private:
			std::atomic_bool _started;
			EventLoop* _loop;
			std::atomic_bool _exiting;
			ThreadInitCallback _callback;
			ThreadFunc _func;
			std::mutex  _mutex;
			std::condition_variable  _cond;
			std::thread* _thread;
		};
	}
}



#endif /* NET_REACTORTHREAD_H_ */
