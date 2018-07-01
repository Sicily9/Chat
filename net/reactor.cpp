/*
 * reactor.cpp
 *
 *  Created on: 2018年2月28日
 *      Author: xc
 */
#include "reactor.h"
#include "handler.h"
#include "poller/basePoller.h"
#include "poller/basePollerFactory.h"
#include "socketop.h"
#include "../utility/log/log.h"

#include <cassert>
#include <sys/syscall.h>
#include <sys/eventfd.h>

using namespace xc;
using namespace xc::net;

const int kPollTimeMs = 10000;
EventLoop * _loopInThread = nullptr;
bool EventLoop::_inited = false;

int createEventfd(){
	int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
	  if (evtfd < 0)
	  {
	    abort();
	  }
	  return evtfd;
}

EventLoop::EventLoop() :
	_looping(false),
	_tid(static_cast<pid_t>(::syscall(SYS_gettid))),
	_quit(false),
	_callingPendingFunctors(false),
	_wakeupFd(createEventfd()),
	_wakeupHandler(new Handler(this, _wakeupFd)),
	_poller(PollerFactory::CreatePoller(PollerFactory::EPOLL)),
	_currentActiveHandler(nullptr)
{
		configInit();
		XC_LOG_DEBUG("eventloop created %d in thread %d ", this, _tid);
		if(_loopInThread){
			XC_LOG_ERROR("another eventloop %d exist in thread %d",_loopInThread, _tid);
		} else {
			_loopInThread = this;
		}
		_wakeupHandler->setReadCallback([this](){
			this->handleRead();
		});
		_wakeupHandler->enableReading();
}


void EventLoop::configInit(){
	if(!_inited){
		_inited = true;
		xc::Log::Config logcfg;
		logcfg._level = xc::Log::Level::XC_DEBUG;
		logcfg._isAsync = false;
		xc::Log::GetInstance()->Init(logcfg);
	}
}

EventLoop::~EventLoop()
{
	XC_LOG_DEBUG("%d eventLoop quit", _tid);
	assert(!_looping);
	_wakeupHandler->disableAll();
	_wakeupHandler->remove();
	::close(_wakeupFd);
	_loopInThread = nullptr;
}

bool EventLoop::isInLoopThread(){
	return _tid == static_cast<pid_t>(::syscall(SYS_gettid));
}

void EventLoop::wakeup()
{
  uint64_t one = 1;
  ssize_t n = socketsop::write(_wakeupFd, &one, sizeof one);
  if (n != sizeof one)
  {

  }
}

void EventLoop::handleRead()
{
  uint64_t one = 1;
  ssize_t n = socketsop::read(_wakeupFd, &one, sizeof one);
  if (n != sizeof one)
  {

  }
}

void EventLoop::quit(){
	_quit = true;
	XC_LOG_DEBUG("%d %d %d",_tid , static_cast<pid_t>(::syscall(SYS_gettid)), _wakeupFd);
	if (!isInLoopThread())
	{
		wakeup();
	}
}

void EventLoop::runInLoop(const Functor& cb)
{
	if (isInLoopThread()){
		cb();
	}
	else
		queueInLoop(cb);
}

void EventLoop::queueInLoop(const Functor& cb)
{
	{
		std::unique_lock<std::mutex> lock(_mutex);
	    _pendingFunctors.push_back(cb);
	}

	if (!isInLoopThread() || _callingPendingFunctors)
	{
		XC_LOG_DEBUG("queInloop");
		wakeup();
	}
}

void EventLoop::loop()
{
	XC_LOG_DEBUG("EventLoop %d start looping ", this);
	assert(!_looping);
	_looping = true;
	_quit = false;
	while(!_quit){
		_activeHandlers.clear();
		_poller->poll(kPollTimeMs, &_activeHandlers);

		for(auto it = _activeHandlers.begin(); it != _activeHandlers.end(); it++){
			_currentActiveHandler = *it;
			_currentActiveHandler->handleEvent();
		}
		_currentActiveHandler = nullptr;
		doPendingFunctors();
	}
	_looping = false;
	XC_LOG_DEBUG("EventLoop %d stop looping ", this);
}

void EventLoop::updateHandler(Handler* handler)
{
	_poller->updateHandler(handler);
}

void EventLoop::removeHandler(Handler* handler)
{
	_poller->removeHandler(handler);
}

bool EventLoop::hasHandler(Handler* handler)
{
	return _poller->hasHandler(handler);
}

void EventLoop::doPendingFunctors()
{
	std::vector<Functor> functors;
	_callingPendingFunctors = true;

	{
		std::unique_lock<std::mutex> lock(_mutex);
		functors.swap(_pendingFunctors);
	}

	for (size_t i = 0; i < functors.size(); ++i)
	{
		functors[i]();
	}
	_callingPendingFunctors = false;
}
