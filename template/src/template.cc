/*
   File name:  template.cc
   Date:       2010/03/30 14:12
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <xc/config.h>
#include <xc/log.h>
#include <xc/error.h>

#include "xc/template.h"

xc::templ::templ_t::templ_t(const xc::templ::env_t& env, xc::templ::output_t& out)
    : _env(env)
{
}

xc::templ::templ_t::~templ_t()
{
}

void xc::templ::templ_t::add_page(char const* name)
{
    LOG_INFO(2, "Add page '%s' to template.", name);

    env.
}



