/*
   File name:  output.cc
   Date:       2010/03/30 15:12
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xc/template/output.h"

xc::templ::output_t::output_t()
{
}

xc::templ::output_t::~output_t()
{
}


xc::templ::html_output_t::html_output_t(xc::ostream_t& out)
    : _out(out)
{
}

xc::templ::html_output_t::~html_output_t()
{
}


