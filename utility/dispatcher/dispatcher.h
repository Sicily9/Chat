#ifndef _BASE_DISPATCHER_IMPL_H
#define _BASE_DISPATCHER_IMPL_H

#include <map>
#include <iostream>
#include "transportData.h"
#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>

namespace xc {
namespace base {

/*------------------------------------------------------------------默认回调函数----------------------------------------------------------------------*/
	//默认回调函数
	void onDefault(const TransportDataPtr& msg);

/*-------------------------------------------------------------------Dispathcer----------------------------------------------------------------------*/
	//消息分发器
	class Dispatcher :public boost::noncopyable{

	public:
		typedef boost::function<void(const TransportDataPtr& message)> ProtobufCallBack;
		explicit Dispatcher(const ProtobufCallBack& _defaultCb);
		~Dispatcher();
		void onMessage(const TransportDataPtr& message);
	//模板必须在.h文件中实现
		void registerMessageCallBack(const google::protobuf::Descriptor* desc,  const ProtobufCallBack& protobufCb);

	private:
		typedef std::map<const google::protobuf::Descriptor*, ProtobufCallBack> CallBackMap;
		CallBackMap _callbackMap;   //　descriptor　-　callback 表

		ProtobufCallBack _defaultCb;  // 默认回调函数
	};


}
}
#endif
	//C++类模板里可以有虚函数  , 成员函数模板不可以是虚函数
	//template <class T> struct AA {
	//	template <class C> virtual void g(C); // error
	//	virtual void f(); // OK
	//};


