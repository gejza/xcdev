

/*
* Copyright
*/

#pragma once
#ifndef _XC_ERROR_H_
#define _XC_ERROR_H_

#include "string.h"
#include "log.h"
#include "text.h"

namespace xc {

class error_t
{
public:
	error_t(const char* format, ...);
	error_t(const xc::string& msg);
	virtual ~error_t() {}
	virtual xc::string message() const
	{
		return _message;
	}
private:
    xc::string _message;
};

class runtime_error_t : public error_t
{
public:
	runtime_error_t(const xc::string& msg);
};

}

#ifndef ERROR
#define ERROR(err, msg...) \
    do { \
        err e(xc::format(msg)); \
        LOG_ERROR(1, msg); \
        throw e; \
    } while (0);
#endif // ERROR

#define RUNTIME_ERROR(msg...) ERROR(xc::runtime_error_t, msg)

#endif // _XC_ERROR_H_
