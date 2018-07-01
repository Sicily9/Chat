/*
 * log_impl.cpp
 *
 *  Created on: 2017年12月8日
 *      Author: xc
 */
#include "log_impl.h"
#include <cstdarg>
#include <iostream>
#include <cstring>
#include <ctime>
#include <sys/time.h>

namespace xc{
	xc::Log* xc::Log::_log = nullptr;
	//单例
	xc::Log* xc::Log::GetInstance() {
		if (!_log) {
			_log = new  xc::LogImpl();
		}
		return _log;
	}

	xc::LogImpl::LogImpl():
		_ready(false),
		_ringBuffer(nullptr),
		_recvThread(nullptr),
		_isInit(false){

	}

	int LogImpl::Init(const Config& config) {
		if (_isInit) {
			return -1;
		}

		_config.CopyFrom(config);

		if (_config._isAsync) {
			_ringBuffer = new xc::RingBuf<char>(100 * DEBUG_BUFFER_MAX_LENGTH);
			_recvThread = new std::thread(
					[this]() {
						char tempBuffer[DEBUG_BUFFER_MAX_LENGTH]{ 0 };
						while (true) {
							std::unique_lock<std::mutex>  lk(_mutex);
							memset(tempBuffer, 0, DEBUG_BUFFER_MAX_LENGTH);
							_cv.wait(lk, [this, &tempBuffer] {return _ringBuffer->Pop(tempBuffer, DEBUG_BUFFER_MAX_LENGTH); });
							std::cout << tempBuffer << std::endl;
						}
					});
			_recvThread->detach();
		}
		_isInit = true;
		return 0;
	}

	void xc::LogImpl::Print(Level level, const char* location, const char* format, ...) {

			if (level < _config._level) {
				return;
			}

			char buf[DEBUG_BUFFER_MAX_LENGTH]{ 0 };
			char allBuf[DEBUG_BUFFER_MAX_LENGTH]{ 0 };

			va_list arg;
			va_start(arg, format);
			vsnprintf(buf, DEBUG_CONTENT_MAX_LENGTH, format, arg);
			va_end(arg);

			FormatStr(level, location, buf, allBuf);

			if (_config._isAsync) {
				std::lock_guard<std::mutex> lk(_mutex);
				if (_ringBuffer->Push(allBuf, DEBUG_BUFFER_MAX_LENGTH)) {
					_cv.notify_one();
				}
			}
			else {
					std::cout << allBuf << std::endl;
			}

		}

		void xc::LogImpl::FormatStr(Level level, const char* location, char * _inBuf ,char* _outBuf) {

			char tempbuf[64];
		    struct timeval tv;
		    struct timezone tz;
		    struct tm *p;
		    gettimeofday(&tv, &tz);
		    p = localtime(&tv.tv_sec);
			sprintf(tempbuf, " %4d/%02d/%02d %02d:%02d:%02d.%03ld \n", 1900+p->tm_year, 1+p->tm_mon, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec, tv.tv_usec/1000);

			std::string levelStr;
			switch (level)
			{
			case xc::Log::XC_INFO:
				levelStr = "XC::INFO: ";
				break;
			case xc::Log::XC_WARNING:
				levelStr = "XC::WARNING: ";
				break;
			case xc::Log::XC_ERROR:
				levelStr = "XC::ERROR: ";
				break;
			case xc::Log::XC_TRACK:
				levelStr = "XC::TRACK: ";
				break;
			case xc::Log::XC_DEBUG:
				levelStr = "XC::DEBUG: ";
				break;
			default:
				break;
			}
			sprintf(_outBuf, "%s%s%s%s", levelStr.c_str(), location, tempbuf, _inBuf);
		}

}

