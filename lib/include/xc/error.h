

/*
* Copyright
*/

#pragma once
#ifndef _XC_ERROR_H_
#define _XC_ERROR_H_

#include "string.h"
#include "log.h"

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

}

#define ERROR(err, msg...) \
    do { \
        err e(msg); \
        LOG_ERROR(1, msg); \
        throw e; \
    } while (0); 


#endif // _XC_ERROR_H_
