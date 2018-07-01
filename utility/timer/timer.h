/*
 * timer.h
 *
 *  Created on: 2017年12月8日
 *      Author: xc
 */
#ifndef UTILITY_TIMER_TIMER_H_
#define UTILITY_TIMER_TIMER_H_

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>

namespace xc{
class Timer : public boost::noncopyable{
	public:
		enum Mode {
			AFTER = 0,
			TICK = 1
		};

		typedef boost::function<void()> TimerFunc;

		virtual ~Timer() {}

		static Timer* Create(TimerFunc func = nullptr, uint32_t millionseconds = 0, Mode mode = TICK);

		virtual int Start() = 0;

		virtual int Stop() = 0;

	};

}
#endif

