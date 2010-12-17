/*
   File name:  env.cc
   Date:       2010/12/15 23:26
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "StdAfx.h"

#include "../include/xc/registry/env.h"

xc::registry::Env_t::Env_t()
{
    _config.readFile("test.cfg");
    _config.lookup("application");
}
