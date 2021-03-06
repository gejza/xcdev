/*
   File name:  scanner.h
   Date:       2010/03/26 19:24
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_TEMPLATE_SCANNER_H_
#define _XC_TEMPLATE_SCANNER_H_
#pragma once

#include <stddef.h>

#include "bytecode.h"

namespace xc {
namespace templ {

/**
 * @short scanner_t
 */
class scanner_t
{
public:
    struct term_t
    {
        inscode_t type;
        const char* text;
        size_t length;
        int line;

        term_t(inscode_t type)
            : type(type), text(""), length(0), line(0) {}

        operator bool () const {
            return type != TI_END;
        }

        const char* name() const {
            return ti_name(type);
        }

        xc::string str() const {
            return xc::string(text, length);
        }

        xc::string sub(size_t left, size_t right) const {
            return xc::string(text + left, length - left - right);
        }

        xc::string get_param() const {
            size_t b = 0;
            size_t e = length - 1;
            while (e && text[e] != '\"' && text[e] != '\'')
                e--;
            if (!e)
                return "";
            while (b < e && text[b] != '\"' && text[b] != '\'')
                b++;
            return sub(b + 1, length - e);
        }
    };

    /**
     * Default constructor
     */
    scanner_t() {}

    /**
     * Destructor
     */
    virtual ~scanner_t() {}

    /** 
     * @short 
     * @return 
     */
    virtual const term_t read() = 0;

    virtual const char* file_name() const = 0;
private:
    scanner_t(const scanner_t&);
    scanner_t& operator=(const scanner_t&);
};


} // namespace templ
} // namespace xc

#endif // _XC_TEMPLATE_SCANNER_H_
/* end of scanner.h */

