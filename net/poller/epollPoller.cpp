/*
 * EpollPoller.cpp
 *
 *  Created on: 2018年3月8日
 *      Author: xc
 */
#include "epollPoller.h"
#include "../../utility/log/log.h"
#include "../handler.h"

#include <boost/static_assert.hpp>
#include <sys/epoll.h>
#include <poll.h>
using namespace xc;
using namespace xc::net;

BOOST_STATIC_ASSERT(EPOLLIN == POLLIN);
BOOST_STATIC_ASSERT(EPOLLPRI == POLLPRI);
BOOST_STATIC_ASSERT(EPOLLOUT == POLLOUT);
BOOST_STATIC_ASSERT(EPOLLRDHUP == POLLRDHUP);
BOOST_STATIC_ASSERT(EPOLLERR == POLLERR);
BOOST_STATIC_ASSERT(EPOLLHUP == POLLHUP);

namespace
{
const int kNew = -1;
const int kAdded = 1;
const int kDeleted = 2;
}

EpollPoller::EpollPoller():
	_epollfd(epoll_create1(EPOLL_CLOEXEC)),//close_on_exec标志，如果执行了exec调用，生成的描述符会被关闭（默认是不关闭的）
	_events(kInitEventListSize){
	if(_epollfd < 0){
		XC_LOG_ERROR("create Epoll failed");
	}
}

EpollPoller::~EpollPoller(){
	close(_epollfd);
}

//IO多路复用器。
int EpollPoller::poll(int timeoutMs, HandlerList* activeHandlers)
{
	int numEvents = epoll_wait(_epollfd, &(*_events.begin()),
						static_cast<int>(_events.size()), timeoutMs);

	int savedErrno = errno;
	if (numEvents > 0){
	    fillActiveChannels(numEvents, activeHandlers);
	    if (static_cast<size_t>(numEvents) == _events.size()) {
	      _events.resize(_events.size()*2);
	    }
	}else if (numEvents == 0){
		//XC_LOG_DEBUG("nothing happened");
	}else{
	    // error happens, log uncommon ones
	    if (savedErrno != EINTR){
	      char * msg = strerror(savedErrno);
	      XC_LOG_ERROR("error:%s",msg);
	    }
	}
	return 0;
}

void EpollPoller::updateHandler(Handler* handler)
{
	const int index = handler->index();

	if(index == kNew || index == kDeleted){
		int fd = handler->fd();
		if (index == kNew){
			assert(_handlers.find(fd) == _handlers.end());
			_handlers[fd] = handler;
		}
		else { // index == kDeleted
			assert(_handlers.find(fd) != _handlers.end());
			assert(_handlers[fd] == handler);
		}
		update(EPOLL_CTL_ADD, handler);
		handler->set_index(kAdded);
		XC_LOG_DEBUG("handler %d add in loop ", fd);
	}else{
		if(handler->isNoneEvent()){
			update(EPOLL_CTL_DEL, handler);
			handler->set_index(kDeleted);
		}else{
			update(EPOLL_CTL_MOD, handler);
		}
	}
}

void EpollPoller::removeHandler(Handler* handler)
{
	int fd = handler->fd();

	int index = handler->index();
	assert(index == kAdded || index == kDeleted);
	size_t n = _handlers.erase(fd);
	XC_LOG_DEBUG("handler %d delete in loop ", fd);
	assert(n == 1);
	if (index == kAdded){
		update(EPOLL_CTL_DEL, handler);
	}
	handler->set_index(kNew);
}

//将发生的事件对应的存入事件活跃链表中（事件处理器链表）
void EpollPoller::fillActiveChannels(int numEvents, HandlerList* activeHandlers) const
{
	for (int i = 0; i < numEvents; ++i){
	    Handler* handler = static_cast<Handler*>(_events[i].data.ptr);

	    int fd = handler->fd();
	    HandlerMap::const_iterator it = _handlers.find(fd);
	    assert(it != _handlers.end());
	    assert(it->second == handler);

	    handler->set_revents(_events[i].events);/*由events（关心的事件） 转换revents(当前活动的事件)
	    									所以配置handler的时候需要设置events,使用提供的 enablereading(),
	    									enablewriting(), disable...等接口.
	    								   */
	    activeHandlers->push_back(handler);
	}
}

void EpollPoller::update(int operation, Handler* handler)
{
	struct epoll_event event;
	bzero(&event, sizeof event);
	event.events = handler->events();
	event.data.ptr = handler;
	int fd = handler->fd();
	if (epoll_ctl(_epollfd, operation, fd, &event) < 0){
			XC_LOG_ERROR("epoll_ctl op =%s, fd =%d" ,operationToString(operation), fd);
	}
}

const char* EpollPoller::operationToString(int op)
{
  switch (op)
  {
    case EPOLL_CTL_ADD:
      return "ADD";
    case EPOLL_CTL_DEL:
      return "DEL";
    case EPOLL_CTL_MOD:
      return "MOD";
    default:
      assert(false && "ERROR op");
      return "Unknown Operation";
  }
}

bool EpollPoller::hasHandler(Handler* handle) const{
	HandlerMap::const_iterator it = _handlers.find(handle->fd());
	return it != _handlers.end() && it->second == handle;
}
