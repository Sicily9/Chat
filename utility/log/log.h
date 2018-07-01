/*
 * log.h
 *
 *  Created on: 2017年12月8日
 *      Author: xc
 */
#ifndef _BASE_LOG_LOG_H
#define _BASE_LOG_LOG_H

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define AT __FILE__ ":" TOSTRING(__LINE__) " "
#include <boost/noncopyable.hpp>

namespace xc {
//注意一个坑，如果可变参数被忽略或为空，format后还有个逗号 所以宏替换时，会导致 多一个逗号而出错误。
//这里的话，默认使可变参数后多一个空字符。
//也可以使用##
#define XC_LOG(LEVEL,FORAMT,...) 				xc::Log::GetInstance()->Print(LEVEL, AT, FORAMT, __VA_ARGS__)
#define XC_LOG_INFO_D(FORAMT,...) 				XC_LOG(xc::Log::Level::XC_INFO,	 FORAMT, __VA_ARGS__)
#define XC_LOG_DEBUG_D(FORAMT, ...)			XC_LOG(xc::Log::Level::XC_DEBUG, FORAMT, __VA_ARGS__)
#define XC_LOG_WARNING_D(FORAMT, ...)		XC_LOG(xc::Log::Level::XC_WARNING, FORAMT, __VA_ARGS__)
#define XC_LOG_ERROR_D(FORAMT, ...)			XC_LOG(xc::Log::Level::XC_ERROR, FORAMT, __VA_ARGS__)
#define XC_LOG_TRACK_D(FORAMT, ...)			XC_LOG(xc::Log::Level::XC_TRACK, FORAMT, __VA_ARGS__)

#define XC_LOG_INFO(...)							XC_LOG_INFO_D(__VA_ARGS__, 		"")
#define XC_LOG_DEBUG(...)					XC_LOG_DEBUG_D(__VA_ARGS__, 		"")
#define XC_LOG_WARNING(...)					XC_LOG_WARNING_D(__VA_ARGS__, 	"")
#define XC_LOG_ERROR(...)					XC_LOG_ERROR_D(__VA_ARGS__,		"")
#define XC_LOG_TRACK(...)						XC_LOG_TRACK_D(__VA_ARGS__,		"")


constexpr int DEBUG_CONTENT_MAX_LENGTH	= 256;
constexpr int DEBUG_BUFFER_MAX_LENGTH	= 512;



class Log :public boost::noncopyable{
public:

	enum Level {
		XC_TRACK = 0,
		XC_INFO = 1,
		XC_DEBUG = 2,
		XC_WARNING = 3,
		XC_ERROR = 4,
	};

	struct Config{
		Level			_level;
		bool			_isAsync;
		Config() {
			_isAsync = false;
			_level = XC_INFO;
		}

		Config(const Config& cfg) {
			CopyFrom(cfg);
		}

		void CopyFrom(const Config& cfg) {
			_level = cfg._level;
			_isAsync = cfg._isAsync;
		}
	};

	static Log* GetInstance();
	virtual int Init(const Config& config) = 0;
	virtual void Print(Level level, const char* location, const char* format, ...) = 0;
	virtual ~Log() {};

protected:
	explicit Log() {}
	static Log* _log;
};


}



#endif



