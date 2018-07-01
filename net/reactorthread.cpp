/*
 * EventLoopThread.cc
 *
 *  Created on: 2018年3月20日
 *      Author: xc
 */
#include "reactorthread.h"
#include "reactor.h"
using namespace xc;
using namespace xc::net;
EventLoopThread::EventLoopThread(const ThreadInitCallback& cb)
  : _loop(nullptr),
	_started(false),
    _exiting(false),
	_thread(nullptr),
    _callback(cb)
{
}
EventLoopThread::~EventLoopThread()
{
  _exiting = true;
  if (_loop != NULL) // not 100% race-free, eg. threadFunc could be running callback_.
  {
    _loop->quit();
  }
}

EventLoop* EventLoopThread::startLoop()
{
  assert(!_started);
  _thread = new std::thread([this](){
	this->Start();
  });
  _thread->detach();
  {
	std::unique_lock<std::mutex> locker(_mutex);
    while (_loop == NULL)
    {
      _cond.wait(locker);
    }
  }

  return _loop;
}

void EventLoopThread::Start()
{
	EventLoop loop;

	if (_callback)
	{
		_callback(&loop);
	}

	{
		std::unique_lock<std::mutex> locker(_mutex);
		_loop = &loop;
		_cond.notify_one();
	}

	loop.loop();
	_loop = NULL;
}



