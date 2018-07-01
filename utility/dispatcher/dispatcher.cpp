/*
 * dispatcher.cpp
 *
 *  Created on: 2017年11月19日
 *      Author: xc
 */
#include "dispatcher.h"
#include "../log/log.h"

xc::base::Dispatcher::Dispatcher(const ProtobufCallBack& _defaultCb):
_defaultCb(_defaultCb){
}

xc::base::Dispatcher::~Dispatcher(){
}

void xc::base::Dispatcher::registerMessageCallBack(const google::protobuf::Descriptor* desc,  const ProtobufCallBack& protobufCb){
		_callbackMap.insert(CallBackMap::value_type(desc, protobufCb));
}

void xc::base::Dispatcher::onMessage(const TransportDataPtr& message) {
	XC_LOG_DEBUG("dispatcher start");
	CallBackMap::const_iterator it = _callbackMap.find(message->_msgData->GetDescriptor());
	if(it == _callbackMap.end()) {
		_defaultCb(message);
		return;
	}

	it->second(message);
}


//默认注册函数
void xc::base::onDefault(const TransportDataPtr& msg) {
	std::cout << "Unknown Message"<<std::endl;
}

