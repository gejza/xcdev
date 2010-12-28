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

#include "../include/xc/resource/env.h"
#include "../include/xc/resource/cdb.h"

xc::resource::Env_t::Env_t()
{
    _config.readFile("test.cfg");
    _config.lookup("application");
}

const xc::resource::Lookup_t& xc::resource::Env_t::get() const
{
    xc::resource::ConstDB_t db("system.cdb");
    RUNTIME_ERROR("Lookup not found.");
    return *((const xc::resource::Lookup_t*)0);
}
