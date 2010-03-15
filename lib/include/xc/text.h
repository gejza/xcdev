
/*
* Copyright
*/

#pragma once
#ifndef _XC_TEXT_H_
#define _XC_TEXT_H_

#include "config.h"
#include "string.h"

namespace xc {
    /**
     * @short Format varargs to string.
     * @param format string format.
     * @return formated string.
     */
    string format(const char *format, ...)
        __attribute__((format(printf, 1, 2)));

    string vformat(const char *format, va_list ap);
} // namespace xc


#endif // _XC_TEXT_H_
