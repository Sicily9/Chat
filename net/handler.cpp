/*
 * Handler.cpp
 *
 *  Created on: 2018年3月7日
 *      Author: xc
 */

#include "reactor.h"
#include "handler.h"
#include "../utility/log/log.h"
#include <poll.h>
#include <sstream>

using namespace xc;
using namespace xc::net;

const int Handler::kNoneEvent = 0;
const int Handler::kReadEvent = POLLIN | POLLPRI;
const int Handler::kWriteEvent = POLLOUT;

Handler::Handler(EventLoop* loop, int fd)
  : _loop(loop),
    _fd(fd),
    _events(0),
    _revents(0),
    _index(-1),
	_tied(false),
    _eventHandling(false),
    _addedToLoop(false)
{
}

void Handler::handleEvent()
{
	_eventHandling = true;
	const char* msg = reventsToString().c_str();
	XC_LOG_DEBUG("event type %s", msg);
	if ((_revents & POLLHUP) && !(_revents & POLLIN)){
		if (_closeCallback) _closeCallback();
	}

	if (_revents & POLLNVAL){
		XC_LOG_ERROR("fd = %d Handle event POLLNVAL", _fd);
	}

	if (_revents & (POLLERR | POLLNVAL)){
		if (_errorCallback) _errorCallback();
	}

	if (_revents & (POLLIN | POLLPRI | POLLRDHUP)){
		if (_readCallback) _readCallback();
	}

	if (_revents & POLLOUT){
		if (_writeCallback) _writeCallback();
	}
	_eventHandling = false;
}

Handler::~Handler()
{
	assert(!_eventHandling);
}

void Handler::update()
{
  _addedToLoop = true;
  _loop->updateHandler(this);
}

void Handler::remove()
{
  assert(isNoneEvent());
  _addedToLoop = false;
  _loop->removeHandler(this);
}

std::string Handler::reventsToString() const
{
  return eventsToString(_fd, _revents);
}

std::string Handler::eventsToString() const
{
  return eventsToString(_fd, _events);
}

void Handler::tie(const boost::shared_ptr<void>& obj){
	_tied = true;
	_tie = obj;
}

std::string Handler::eventsToString(int fd, int ev)
{
  std::ostringstream oss;
  oss << fd << ": ";
  if (ev & POLLIN)
    oss << "IN ";
  if (ev & POLLPRI)
    oss << "PRI ";
  if (ev & POLLOUT)
    oss << "OUT ";
  if (ev & POLLHUP)
    oss << "HUP ";
  if (ev & POLLRDHUP)
    oss << "RDHUP ";
  if (ev & POLLERR)
    oss << "ERR ";
  if (ev & POLLNVAL)
    oss << "NVAL ";

  return oss.str().c_str();
}
