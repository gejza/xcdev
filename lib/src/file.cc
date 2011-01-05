/*
   File name:  file.cc
   Date:       2010/03/25 10:39
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xc/file.h"
#include "xc/error.h"

xc::file_t::file_t()
    : _file(NULL)
{
}

xc::file_t::file_t(const char* name, const char* mode)
    : _name(name), _file(NULL)
{
    this->open(name, mode);
}

xc::file_t::~file_t()
{
    if (_file) {
        ::fclose(_file);
        XC_DBG("Close file `%s'", _name.c_str());
    }
}

void xc::file_t::open(const char* name, const char* mode)
{
    if (_file) {
        ::fclose(_file);
        XC_DBG("Close file `%s'", _name.c_str());
    }
    _file = ::fopen(name, mode);
    if (!_file)
        ERROR(xc::error_t, "Failed open file `%s'", name);
    XC_DBG("Open file `%s' with mode %s", name, mode);
}

