/*
 * msgqueue.h
 *
 *  Created on: 2017年12月6日
 *      Author: xc
 */
#ifndef UTILITY_MSGHANDLER_MSGHANDLER_H_
#define UTILITY_MSGHANDLER_MSGHANDLER_H_

#include <stdint.h>
#include <atomic>
#include <list>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <google/protobuf/message.h>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include "../dispatcher/dispatcher.h"
using namespace xc::net;

namespace xc{
namespace base{
	class MessageHandler :public boost::noncopyable {
	public:
		explicit MessageHandler(int _maxsize);
		~MessageHandler();

		//获取消息队列的一些状态
		bool empty(); //判断是否为空
		bool full(); 	//判断是否为满
		size_t size(); //得到当前的任务数量

		xc::base::Dispatcher& getDispatcher();

		//向消息队列中插入，删除消息
		void push(const TransportDataPtr& x);
		void pop(TransportDataPtr& x); //重载的Take方法执行流程一致，只是取出队列中最前面的任务

		//消息处理器的开关。
		void Stop();
		void Run();//消息队列开始接收消息

	private:
		std::list<TransportDataPtr>  _queue;		//用一个链表存储消息

		std::mutex						 _exitMutex; //用于线程安全退出的锁
		std::mutex   						 _mutex;        	//C++11的互斥锁 用于线程同步
		std::condition_variable 			 	_notEmpty; 		//C++11的条件变量,用于保证线程执行的次序
		std::condition_variable 			 	_notFull;  		//同上
		std::condition_variable 			 	_stop;  		//同上
		int16_t 						_maxSize;      		//队列中允许保存的最大任务数
		std::atomic_bool 					_atomicNeedStop;   	//用于终止队列的标志，默认为false(设为原子变量)

		std::thread*						 _recvThread;		//接收消息线程
		xc::base::Dispatcher  			 		_dispatcher;		//消息分发器

		bool							_isSignaled;		//用于条件变量信号的保护。
	};
}
}

#endif /* UTILITY_MSGHANDLER_MSGHANDLER_H_ */
