/*
   File name:  error.cc
   Date:       2010/03/25 14:27
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xc/error.h"
#include "xc/text.h"

xc::error_t::error_t(const char* format, ...)
{
    va_list vl;
    va_start(vl, format);
    _message = xc::vformat(format, vl);
    va_end(vl);
}

xc::error_t::error_t(const xc::string& msg)
    : _message(msg)
{
}



