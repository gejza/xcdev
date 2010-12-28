/*
   File name:  web.cc
   Date:       2010/12/14 15:48
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "StdAfx.h"

#include "../include/xc/resource/web.h"
#include "../include/xc/resource/lookup.h"

std::string xc::resource::PHPIncludeTable_t::find_class(const std::string& name)
{
    const xc::resource::Lookup_t& l = _config.get();
    xc::data_t key(reinterpret_cast<const uint8_t*>(name.data()), name.size());
    xc::data_t value(0,0);
    if (l.lookup(key, value)) {
        return std::string((const char*)value.data(), value.size());
    }
    return "";
}

