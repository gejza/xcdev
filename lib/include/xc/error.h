

/*
* Copyright
*/

#pragma once
#ifndef _XC_ERROR_H_
#define _XC_ERROR_H_

#include "string.h"

namespace xc {
class exception
{
protected:
	int status;
	xc::string message;
public:
	exception(int status, const char* msg = "Unknown Exception")
		:status(status), message(msg) {}
	virtual ~exception() {}
	const char* what() const
	{
		return message.c_str();
	}
};

}

#endif // _XC_ERROR_H_
