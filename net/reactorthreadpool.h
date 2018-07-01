/*
 * EventLoopThreadPool.h
 *
 *  Created on: 2018年3月20日
 *      Author: xc
 */

#ifndef NET_REACTORTHREADPOOL_H_
#define NET_REACTORTHREADPOOL_H_
#include <vector>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <thread>

namespace xc{
	namespace net{
		class EventLoop;
		class EventLoopThread;
		class EventLoopThreadPool: public boost::noncopyable{
		public:
			typedef boost::function<void(EventLoop*)> ThreadInitCallback;

			EventLoopThreadPool(EventLoop* baseLoop);
			~EventLoopThreadPool();
			void setThreadNum(int numThreads) { _numThreads = numThreads; }
			void start(const ThreadInitCallback& cb = ThreadInitCallback());
			// valid after calling start()
			EventLoop* getNextLoop();
			// valid after calling start()
			std::vector<EventLoop*> getAllLoops();

			private:

			EventLoop* _baseLoop;
			bool _started;
			int _numThreads;
			int _next;
			boost::ptr_vector<EventLoopThread> _threads;
			std::vector<EventLoop*> _loops;
		};
	}
}






#endif /* NET_REACTORTHREADPOOL_H_ */
