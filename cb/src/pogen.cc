/*
   File name:  pogen.cc
   Date:       2010/12/06 01:45
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gettext-po.h>

#include "pogen.h"

///////////////////////////////////////////
xc::POGen_t::POGen_t(const char* fn)
{
    _file = ::fopen(fn, "wt");
    if (_file == NULL)
        error (EXIT_FAILURE, errno, "couldn't create the POT file %s", fn);
}

xc::POGen_t::~POGen_t()
{
    ::fclose(_file);
}

void xc::POGen_t::add(const char* msg)
{
    ::fprintf(_file, "msgid \"%s\"\n", msg);
    ::fprintf(_file, "msgstr \"\"\n");
    ::fprintf(_file, "\n");
}

