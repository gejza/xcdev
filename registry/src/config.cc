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

#include "../include/xc/registry/config.h"

const xc::registry::Lookup_t& xc::registry::Config_t::get() const
{
    RUNTIME_ERROR("Lookup not found.");
    return *((const xc::registry::Lookup_t*)0);
}

