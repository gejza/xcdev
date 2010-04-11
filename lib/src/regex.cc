/*
   File name:  regex.cc
   Date:       2010/03/25 07:07
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <string.h>
#include <pcre.h>

#include <xc/log.h>
#include "xc/regex.h"

////////////////////////////////////////////////
// regex_t
xc::regex_t::~regex_t()
{
    if (_re)
        ::pcre_free(_re);
}

void xc::regex_t::_compile(const char* exp)
{
    const char* err;
    int erro;

    if (_re)
        ::pcre_free(_re);
    _re = 0x0;
    if (!exp)
        return;
    _re = ::pcre_compile(exp, 0, &err, &erro, NULL);
    /* Compilation failed: print the error message and exit */
    if (_re == NULL) {
        LOG_ERROR(3, "Compile regex `%s' failed on %d: %s", exp, erro, err);
        return;
    }
}

bool xc::regex_t::exec(const char* str, size_t len) const
{
    if (!_re)
       return false;
    if (!len)
        len = ::strlen(str);
    int rc = ::pcre_exec(reinterpret_cast<const pcre*>(_re), NULL, str, len, 0, 0, NULL, 0);
    if (rc >= 0)
        return true;

    switch (rc) {
    case PCRE_ERROR_NOMATCH:
        return false;
    default:
        LOG_ERROR(1, "Matching error %d\n", rc);
    };
    
    return false;
}



