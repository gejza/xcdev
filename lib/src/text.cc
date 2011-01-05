/*
   File name:  text.cc
   Date:       2010/03/25 06:37
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <vector>

#include <xc/log.h>
#include "xc/text.h"


////////////////////////////////////////////////
// Functions

xc::string xc::format(const char *fmt, ...) {
    va_list ap;
    va_start (ap, fmt);
    std::string buf = vformat (fmt, ap);
    va_end (ap);
    return buf;
}

xc::string xc::vformat(const char *fmt, va_list ap) {
    // Allocate a buffer on the stack that's big enough for us almost
    // all the time.
    size_t size = 1024;
    char buf[size];

    // Try to vsnprintf into our buffer.
    va_list apcopy;
    va_copy (apcopy, ap);
    int needed = vsnprintf (&buf[0], size, fmt, ap);
    // NB. On Windows, vsnprintf returns -1 if the string didn't fit the
    // buffer.  On Linux & OSX, it returns the length it would have needed.

    if (needed <= size && needed >= 0) {
        // It fit fine the first time, we're done.
        return std::string (&buf[0]);
    } else {
        // vsnprintf reported that it wanted to write more characters
        // than we allotted.  So do a malloc of the right size and try again.
        // This doesn't happen very often if we chose our initial size
        // well.
        std::vector <char> buf;
        size = needed;
        buf.resize (size);
        needed = vsnprintf (&buf[0], size, fmt, apcopy);
        return std::string (&buf[0]);
    }
}

xc::string xc::human(const char* str, size_t limit)
{
	return xc::human(str, ::strlen(str), limit);
}

xc::string xc::human(const void* data, size_t size, size_t limit)
{
    xc::string ret;
	const char* str = reinterpret_cast<const char*>(data);
    for (size_t i=0;i < size; i++) {
        const ssize_t rem = limit - ret.size();
        if (rem < 1) {
            // over limit, truncate
            ret.resize(limit - 3);
            ret.append("...");
            break;
        }
        if (::isprint(str[i])) {
            /*const char* s = str;
            // skip to all printable
            while (::isprint(*s)) s++;
            size_t len = s - str;
            if (len > rem)
                len = rem;
            ret.append(str, len);
            str += len;*/
			ret.push_back(str[i]);
            continue;
        }
        switch (str[i]) {
        case '\n':
            ret.append("<CR>");
            break;
        case '\r':
            ret.append("<LF>");
            break;
        default:
            ret.append("?");
            break;
        };
    }
    return ret;
}

