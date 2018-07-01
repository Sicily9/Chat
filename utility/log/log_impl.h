/*
 * log_impl.h
 *
 *  Created on: 2017年12月8日
 *      Author: xc
 */
#ifndef  _XC_LOG_IMPL_H_
#define _XC_LOG_IMPL_H_

#include <thread>
#include <condition_variable>
#include <mutex>

#include "../log/log.h"
#include "../ringbuffer/ringbuffer.h"
namespace xc{
class LogImpl : public Log {

	public:
		explicit LogImpl();
		int Init(const Config& config) override;
		void Print(Level level, const char* location, const char* format, ... ) override;
		~LogImpl() {}

	private:
		Config _config;
		std::thread *_recvThread;
		xc::RingBuf<char>	*_ringBuffer;
		std::mutex _mutex;
		std::condition_variable_any _cv;
		bool _ready;
		bool _isInit;

		void FormatStr(Level level, const char* location, char * _inBuf, char* _outBuf);
	};

}


#endif



