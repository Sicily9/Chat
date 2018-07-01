/*
 * Channel.h
 *
 *  Created on: 2018年3月7日
 *      Author: xc
 */

#ifndef NET_HANDLER_H_
#define NET_HANDLER_H_

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace xc{
	namespace net{
		class EventLoop;

		class Handler : public boost::noncopyable {
		public:
			typedef boost::function<void()> EventCallback;
			//typedef boost::function<void()> ReadEventCallback;

			Handler(EventLoop* reactor, int fd);
			~Handler();

			void handleEvent();
			void setReadCallback(const EventCallback& cb){ _readCallback = cb; }
			void setWriteCallback(const EventCallback& cb){ _writeCallback = cb; }
			void setCloseCallback(const EventCallback& cb){ _closeCallback = cb; }
			void setErrorCallback(const EventCallback& cb){ _errorCallback = cb; }

			int fd() const { return _fd; }
			int events() const { return _events; }
			void set_revents(int revt) { _revents = revt; } // used by pollers
			bool isNoneEvent() const { return _events == kNoneEvent; }
			void tie(const boost::shared_ptr<void>&);
			void enableReading() { _events |= kReadEvent; update(); }
			void disableReading() { _events &= ~kReadEvent; update(); }
			void enableWriting() { _events |= kWriteEvent; update(); }
			void disableWriting() { _events &= ~kWriteEvent; update(); }
			void disableAll() { _events = kNoneEvent; update(); }
			bool isWriting() const { return _events & kWriteEvent; }
			bool isReading() const { return _events & kReadEvent; }

			// for debug
			std::string reventsToString() const;
			std::string eventsToString() const;
			// for Poller
			int index() { return _index; }
			void set_index(int idx) { _index = idx; }

			EventLoop* ownerLoop() { return _loop; }
			void remove();
		private:
			static std::string eventsToString(int fd, int ev);
			void update();
			void handleEventWithGuard();

			static const int kNoneEvent;
			static const int kReadEvent;
			static const int kWriteEvent;

			EventLoop* _loop;
			const int  _fd;
			int        _events;
			int        _revents; // it's the received event types of epoll or poll
			int        _index; // used by Poller.
			bool _tied;
			boost::weak_ptr<void> _tie;
			bool _eventHandling;
			bool _addedToLoop;
			EventCallback _readCallback;
			EventCallback _writeCallback;
			EventCallback _closeCallback;
			EventCallback _errorCallback;
		};
	}
}



#endif /* NET_HANDLER_H_ */
