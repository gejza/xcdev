
#include <string.h>
#include "xc/error.h"

XC_API const char* xc::error::ExceptionString(const char* message)
{
	return strdup(message);
}

XC_API void xc::error::FreeExceptionString(const char* message)
{
	return free((void*)message);
}

