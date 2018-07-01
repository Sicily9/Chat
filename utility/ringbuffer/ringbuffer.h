/*
 * ringbuffer.h
 *
 *  Created on: 2017年12月10日
 *      Author: xc
 */
#ifndef UTILITY_RINGBUFFER_RINGBUFFER_H_
#define UTILITY_RINGBUFFER_RINGBUFFER_H_
#include <cstdint>
#include <cstring>
#include <boost/noncopyable.hpp>

namespace xc{
template <typename T>
class RingBuf :public boost::noncopyable{
public:

	explicit RingBuf(int32_t len):
		_len(len+1),_begIndex(0),_endIndex(0) {
		_buf = new T[_len]{ 0 };
	}

	void Reset() {
		_begIndex = 0;
		_endIndex = 0;
		memset(_buf, 0, _len * sizeof(T));
	}

	bool Push(const T* buf, int32_t len) {
		if ( len >= _len - GetContentLen()) {
			return false;
		}
		if (_len - _endIndex >= len) {
			memcpy(_buf + _endIndex, buf, len * sizeof(T));
			_endIndex += len;
			_endIndex %= _len;
		} else {
			int afterLen = _len - _endIndex;
			memcpy(_buf + _endIndex, buf, afterLen * sizeof(T));
			int frontLen = len - afterLen;
			memcpy(_buf, buf + afterLen, frontLen * sizeof(T));
			_endIndex = frontLen;
		}
		return true;
	}

	T* Pop(int32_t len) {
		if ((_begIndex + len) % _len == _endIndex || len > _len) {
			return nullptr;
		}
		T* tempBuf = new T[len];
		if (_endIndex > _begIndex){
			memcpy(tempBuf, _buf + _begIndex, len * sizeof(T));
			_begIndex += len;
		} else {
			int afterLen = _len - _begIndex;
			if (afterLen >= len) {
				memcpy(tempBuf, _buf + _begIndex, len * sizeof(T));
				_begIndex += len;
				_begIndex %= _len;
			} else {
				memcpy(tempBuf, _buf + _begIndex, afterLen * sizeof(T));
				int frontLen = len - afterLen;
				memcpy(tempBuf + afterLen, _buf, frontLen * sizeof(T));
				_begIndex = frontLen;
			}
		}
		return tempBuf;
	}

	bool Pop(T* in_buf, int32_t len) {
		if (GetContentLen() < len || len > _len){
			return false;
		}
		if (_endIndex > _begIndex){
			memcpy(in_buf, _buf + _begIndex, len * sizeof(T));
			_begIndex += len;
		} else {
			int afterLen = _len - _begIndex;
			if (afterLen >= len) {
				memcpy(in_buf, _buf + _begIndex, len * sizeof(T));
				_begIndex += len;
				_begIndex %= _len;
			} else {
				memcpy(in_buf, _buf + _begIndex, afterLen * sizeof(T));
				int frontLen = len - afterLen;
				memcpy(in_buf + afterLen, _buf, frontLen * sizeof(T));
				_begIndex = frontLen;
			}
		}
		return true;
	}

	int32_t TryPop(T* in_buf, int32_t len) {
		const int contentLen = GetContentLen();
		if (contentLen < len) {
			Pop(in_buf, contentLen);
			return contentLen;
		}
		Pop(in_buf, len);
		return len;
	}

	bool Front(int32_t len) {
		if ((_begIndex + len) % _len == _endIndex || len > _len){
			return false;
		}
		if (_endIndex > _begIndex){
			_begIndex += len;
		}
		else {
			int afterLen = _len - _begIndex;
			if (afterLen >= len) {
				_begIndex += len;
				_begIndex %= _len;
			}
			else {
				int frontLen = len - afterLen;
				_begIndex = frontLen;
			}
		}
		return true;
	}

	~RingBuf() {
		if(_buf) {
			delete[]_buf;
			_buf = nullptr;
		}
	}

private:
	int32_t _begIndex;
	int32_t _endIndex;
	int32_t _len;
	T*      _buf;

	int GetContentLen() const {
		if (_begIndex < _endIndex){
			return (_endIndex - _begIndex);
		} else if (_begIndex > _endIndex) {
			return (_len - (_begIndex - _endIndex));
		}
		return 0;
	}
};
}



#endif



