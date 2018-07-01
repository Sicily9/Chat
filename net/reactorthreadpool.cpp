/*
 * EventLoopThreadPool.cpp
 *
 *  Created on: 2018年3月20日
 *      Author: xc
 */

#include "reactorthreadpool.h"
#include "reactorthread.h"
#include "reactor.h"

using namespace xc;
using namespace xc::net;

EventLoopThreadPool::EventLoopThreadPool(EventLoop* baseLoop)
  : _baseLoop(baseLoop),
    _started(false),
    _numThreads(0),
    _next(0)
{
}

EventLoopThreadPool::~EventLoopThreadPool()
{
  // Don't delete loop, it's stack variable
}

void EventLoopThreadPool::start(const ThreadInitCallback& cb)
{
	assert(!_started);

	_started = true;

	for (int i = 0; i < _numThreads; ++i)
	{
		EventLoopThread* t = new EventLoopThread(cb);
		_threads.push_back(t);
		_loops.push_back(t->startLoop());
	}
	if (_numThreads == 0 && cb)
	{
		cb(_baseLoop);
	}
}

EventLoop* EventLoopThreadPool::getNextLoop()
{
  assert(_started);
  EventLoop* loop = _baseLoop;

  if (!_loops.empty())
  {
    // round-robin
    loop = _loops[_next];
    ++_next;
    if (static_cast<size_t>(_next) >= _loops.size())
    {
      _next = 0;
    }
  }
  return loop;
}

std::vector<EventLoop*> EventLoopThreadPool::getAllLoops()
{
  assert(_started);
  if (_loops.empty())
  {
    return std::vector<EventLoop*>(1, _baseLoop);
  }
  else
  {
    return _loops;
  }
}
