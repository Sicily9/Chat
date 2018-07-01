/*
 * timer.h
 *
 *  Created on: 2017年12月8日
 *      Author: xc
 */
#ifndef UTILITY_TIMER_TIMER_IMPL_H_
#define UTILITY_TIMER_TIMER_IMPL_H_
#include <signal.h>
#include <time.h>

#include "../../utility/timer/timer.h"

namespace xc{
	class TimerImpl : public xc::Timer {
	public:
		TimerImpl(TimerFunc func, uint32_t millionseconds, Mode mode);
		int Start() override;
		int Stop() override;
		~TimerImpl();

	private:
		TimerFunc _func;
		Mode _mode;
		uint32_t _millionSeconds;

        timer_t _timerID;
        struct sigevent _evp;
        struct itimerspec _it;

        static void timer_thread(union sigval v);
	};


}

#endif /* UTILITY_TIMER_TIMER_IMPL_H_ */
