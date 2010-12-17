/*
   File name:  web.h
   Date:       2010/12/14 15:26
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_REGISTRY_WEB_H_
#define _XC_REGISTRY_WEB_H_
#pragma once

#include "config.h"

namespace xc {
namespace registry {

class PHPIncludeTable_t
{
public:
    PHPIncludeTable_t(xc::registry::Config_t& config)
        : _config(config) {}
    virtual ~PHPIncludeTable_t() {}

    std::string find_class(const std::string& name);

    xc::registry::Config_t& _config;
};

} // namespace registry
} // namespace xc

#endif // _XC_REGISTRY_WEB_H_
/* end of web.h */
