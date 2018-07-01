#ifndef PROTOBUF_CODEC_H
#define PROTOBUF_CODEC_H

#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>

#include <string>
#include <arpa/inet.h>  // htonl, ntohl
#include <stdint.h>
#include <iostream>
#include <iconv.h>
#include "../proto/TcpProto.pb.h"
#include "../../../utility/log/log.h"
const int kHeaderLen = sizeof(int32_t);

inline bool convertGbk2Utf(const std::string& instr, std::string& outstr)
{
	iconv_t  gbk2UtfDescriptor;
	gbk2UtfDescriptor = iconv_open("UTF-8", "GBK");
	size_t inlen = instr.length();
	char* in = const_cast<char*>(instr.c_str());
	size_t outlen = inlen * 2 + 1; // inlen * 1.5 + 1 >= outlen >= inlen + 1
	char* outbuf = (char*)::malloc(outlen);
	char* out = outbuf;
	memset(outbuf, 0x0, outlen);
	if((size_t)-1 == iconv(gbk2UtfDescriptor, &in, &inlen, &out, &outlen))
	{
		::free(outbuf);
		return false;
	}
	outstr.clear();
	outstr.append(outbuf);
	::free(outbuf);
	return true;
}


inline std::string encode(const google::protobuf::Message& message)
{
	std::string result;

	result.resize(kHeaderLen);

	const std::string& typeName = message.GetTypeName();
	int32_t nameLen = static_cast<int32_t>(typeName.size());
	int32_t be32 = ::htonl(nameLen);
	result.append(reinterpret_cast<char*>(&be32), sizeof be32);
	result.append(typeName.c_str(), nameLen);

	bool succeed = message.AppendToString(&result);
	if (succeed){
		const char* begin = result.c_str() + kHeaderLen;

		int32_t len = ::htonl(result.size() - kHeaderLen);
		std::copy(reinterpret_cast<char*>(&len),
			  reinterpret_cast<char*>(&len) + sizeof len,
			  result.begin());
	}

	else{
		result.clear();
	}

	return result;
}

inline google::protobuf::Message* createMessage(const std::string& type_name)
{
	google::protobuf::Message* message = NULL;
	const google::protobuf::Descriptor* descriptor =
	google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(type_name);
	if (descriptor){
		const google::protobuf::Message* prototype =
		google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);
		if (prototype){
			message = prototype->New();
		}
	}
	return message;
}

inline int32_t asInt32(const char* buf)
{
	int32_t be32 = 0;
	::memcpy(&be32, buf, sizeof(be32));
	return ::ntohl(be32);
}

inline google::protobuf::Message* decode(const std::string& buf)
{

	google::protobuf::Message* result = NULL;
	int32_t len = static_cast<int32_t>(buf.size());
	if (len >= 10) {
		const char* begin = buf.c_str();
		int32_t nameLen = asInt32(buf.c_str());

		//if (nameLen >= 2 && nameLen <= len - 2*kHeaderLen) {
		if (nameLen >= 2 && nameLen <= len - kHeaderLen) {

			std::string typeName(buf.begin() + kHeaderLen, buf.begin() + kHeaderLen + nameLen);
			google::protobuf::Message* message = createMessage(typeName);
			XC_LOG_DEBUG("%s", typeName.c_str());
			if (message) {
				const char* data = buf.c_str() + kHeaderLen + nameLen;
				int32_t dataLen = len - nameLen - kHeaderLen;
				//for(int i = 0; i < dataLen; i++){
				//	XC_LOG_DEBUG("%d", (int)data[i]);
				//}
				if (message->ParseFromArray(data, dataLen)) {
					result = message;
					//XC_LOG_DEBUG("%d, %s" ,((LoginReq *)result)->id(), ((LoginReq *)result)->password().c_str() );
					//LoginReq * req = (LoginReq *)result;
					//XC_LOG_DEBUG("debug nameLen: %d type:%s id: %d pwd: %s",nameLen , typeName.c_str(), req->id(), req->password().c_str());
				} else {
					XC_LOG_DEBUG("%d, %s" ,((LoginReq *)message)->id(), ((LoginReq *)message)->password().c_str() );
					// parse error
					XC_LOG_DEBUG("parse error");
					delete message;
				}
			} else{
				// unknown message type
			}
		} else {
			// invalid name len
		}
	}
	return result;
}

#endif // PROTOBUF_CODEC_H
