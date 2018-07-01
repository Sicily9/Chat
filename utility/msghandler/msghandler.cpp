/*
 * msghandler.cpp
 *
 *  Created on: 2017年12月7日
 *      Author: xc
 */
#include "../../utility/msghandler/msghandler.h"
#include "../log/log.h"
#include "../../ChatServer/include/room/room.h"
namespace xc{
namespace base{
	xc::base::MessageHandler::MessageHandler(int maxsize):
			_atomicNeedStop(false),
			_maxSize(maxsize),
			_recvThread(nullptr),
			_dispatcher(onDefault),
			_isSignaled(false){
	}

	xc::base::MessageHandler::~MessageHandler(){
		Stop();
	}

	bool xc::base::MessageHandler::empty() {//判断是否为空
		return _queue.empty();
	}

	bool xc::base::MessageHandler::full() { 	//判断是否为满
		return _queue.size() == _maxSize;
	}

	size_t xc::base::MessageHandler::size() { //得到当前的任务数量
		return _queue.size();
	}

	xc::base::Dispatcher& xc::base::MessageHandler::getDispatcher(){
		return _dispatcher;
	}

	void xc::base::MessageHandler::push(const TransportDataPtr& x) {
		std::unique_lock<std::mutex> locker(_mutex);
		_notFull.wait(locker, [this]{return _atomicNeedStop || !full();} );

		if(_atomicNeedStop)
			return;

		_queue.push_back(x);

		if(_queue.size() == 1){
			_notEmpty.notify_one();//唤醒pop中的wait
		}

	}

	void xc::base::MessageHandler::pop(TransportDataPtr& x) { //重载的Take方法执行流程一致，只是取出队列中最前面的任务

		std::unique_lock<std::mutex> locker(_mutex);
		_notEmpty.wait(locker, [this]{return _atomicNeedStop || !empty();} ); //可以预防虚假唤醒（wait外部包了一层while循环）

		if(_atomicNeedStop)
			return ;
		x = _queue.front();
		_queue.pop_front(); //取出任务

		if(_queue.size() == _maxSize - 1){
			_notFull.notify_one();//唤醒push中的wait
		}
	}

//TODO: 关于智能指针的用法调研清楚。
//也可以构想用一个线程池，开N个线程 共享这个消息队列, 获取消息。
//这里 是run函数里单独整一个线程出来 来取消息
	void xc::base::MessageHandler::Run() {
		_atomicNeedStop = false;
		_isSignaled = false;
		//开启接收消息线程
		_recvThread = new std::thread([this]() {
			TransportDataPtr msg;
			while(!_atomicNeedStop) {

				pop(msg);			//从消息队列中取出消息
				if(_atomicNeedStop)
					break;

				_dispatcher.onMessage(msg); //去运行具体的事件。
			}
			std::unique_lock<std::mutex> locker(_exitMutex);
			_isSignaled = true;
			_stop.notify_all();
		});
		_recvThread->detach();
	}

//线程安全退出 done
	void xc::base::MessageHandler::Stop() {
		_atomicNeedStop = true;//设置标志 原子变量。下面唤醒线程之后,线程能立刻拿到锁退出.
		_notFull.notify_all(); //唤醒正在等待Take和Put的线程，然后所有线程依此拿锁return
		_notEmpty.notify_all();

		std::unique_lock<std::mutex> locker(_exitMutex);
		while(!_isSignaled){
			_stop.wait(locker);
		}
	}
//注意虚假唤醒。
}
}


