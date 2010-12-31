/*
   File name:  config.cc
   Date:       2010/12/14 18:32
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "StdAfx.h"

#include "../include/xc/rd/config.h"

extern "C" {
    void xcrd_version()
    {
    }
}

const xc::resource::Lookup_t& xc::resource::Config_t::get() const
{
    RUNTIME_ERROR("Lookup not found.");
    return *((const xc::resource::Lookup_t*)0);
}

