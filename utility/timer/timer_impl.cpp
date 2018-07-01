/*
 * timer.cpp
 *
 *  Created on: 2017年12月8日
 *      Author: xc
 */
#include "../../utility/timer/timer_impl.h"

namespace xc{
	Timer* Timer::Create(xc::Timer::TimerFunc func, uint32_t millionseconds, xc::Timer::Mode mode) {
		return new xc::TimerImpl(func, millionseconds, mode);
	}

    void TimerImpl::timer_thread(union sigval v)
    {
        ((TimerImpl*)v.sival_ptr)->_func();
    }

	TimerImpl::TimerImpl(xc::Timer::TimerFunc func, uint32_t millionseconds, xc::Timer::Mode mode) :
		_func(func),
		_mode(mode),
		_millionSeconds(millionseconds),
		_timerID(0){

        memset(&_evp, 0, sizeof(struct sigevent));
        _evp.sigev_notify = SIGEV_THREAD;
        _evp.sigev_notify_function = timer_thread;
        _evp.sigev_signo = SIGALRM;
        _evp.sigev_value.sival_ptr = this;

        _it.it_value.tv_sec = (long)_millionSeconds / 1000;
        _it.it_value.tv_nsec = (long)( _millionSeconds % 1000 ) * 1000 * 1000;
        _it.it_interval.tv_sec = 0;
        _it.it_interval.tv_nsec = 0;
	}

	TimerImpl::~TimerImpl() {
        Stop();
	}

    int TimerImpl::Start() {
        if (timer_create(CLOCK_REALTIME, &_evp, &_timerID) == -1) {
            return -1;
        }

        if(_func) {
            if (_mode == TICK) {
                _it.it_interval.tv_sec = (long) _millionSeconds / 1000;
                _it.it_interval.tv_nsec = (long) (_millionSeconds % 1000) * 1000 * 1000;
            }
        }

        if (timer_settime(_timerID, 0, &_it, NULL) == -1) {
            return -1;
        }
        return 0;
    }


	int TimerImpl::Stop() {
        timer_delete(_timerID);
		return 0;
	}


}
