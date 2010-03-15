
/*
* Copyright
*/

#pragma once
#ifndef _STREAM_H_
#define _STREAM_H_

#include <xc/stream.h>
#include <xc/config.hpp>

#ifdef XC_LIB_STD_COMPATIBLE
//#include <stream>
#endif // XC_LIB_STD_COMPATIBLE

namespace xc {

class String;

class Stream
{
public:
	virtual size_t Read(char* buff, size_t num) const = 0;
	XC_API const String ToString() const;
};

class CStream : public Stream
{
public:
	virtual ~CStream() {}
	virtual size_t Read(char* buff, size_t num) const;
};

class StreamFilter : public Stream
{
protected:
	void Out(const char* str, size_t s);
public:
	virtual bool Filter(const char* param) = 0;
};


#ifdef XC_LIB_STD_COMPATIBLE
typedef Stream stream;
#endif // XC_LIB_STD_COMPATIBLE

} // namespace xc

#endif // _STREAM_H_
