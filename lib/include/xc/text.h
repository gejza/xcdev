
/*
* Copyright
*/

#pragma once
#ifndef _XC_TEXT_H_
#define _XC_TEXT_H_

#include <stdarg.h>
#include <stdio.h>

#include "string.h"
#include "regex.h"

namespace xc {

    /**
     * @short Format varargs to string.
     * @param format string format.
     * @return formated string.
     */
    string format(const char *format, ...)
        __attribute__((format(printf, 1, 2)));

    /**
     * @short Format varargs to string.
     * @param format string format.
     * @return formated string.
     */
    string vformat(const char *format, va_list ap);

    /** 
     * @short Convert string to human reading (example for debug)
     * @param str String
     * @param limit Max length. If longer, ending with ...
     * @return Human string
     */
    string human(const char* str, size_t limit);

    inline void i18n(const char* locale)
    {
#ifdef ENABLE_NLS
#ifdef LOCALEDIR
        ::bindtextdomain(PACKAGE, LOCALEDIR);
#endif
        ::textdomain(PACKAGE);
        ::setlocale(LC_ALL, locale);
#endif
    }

    class colored_t
    {
    public:
        colored_t(FILE* out, int color, int style = 1)
            : _out(out) {

            if (color && ::isatty(_out->_fileno)) {
                //if (background)
                //    ::fprintf(_out, "\033[1;%dm", color);
                //else
                    ::fprintf(_out, "\033[%d;%dm", style, color);
            }
        }

        colored_t(FILE* out, const char* scolor)
            : _out(out) {

            if (scolor && ::isatty(_out->_fileno)) {
                ::fputs(scolor, _out);
            }
        }

        ~colored_t() {

            if (::isatty(_out->_fileno))
                ::fprintf(_out, "\033[0m");
        }
    private:
        FILE* _out;
    };

} // namespace xc


#endif // _XC_TEXT_H_
