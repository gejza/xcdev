/*
   File name:  config.h
   Date:       2010/12/14 17:48
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_REGISTRY_CONFIG_H_
#define _XC_REGISTRY_CONFIG_H_
#pragma once

namespace xc {
namespace resource {

class Lookup_t;

class Config_t
{
public:
    Config_t() {}

    const Lookup_t& get() const;
};

} // namespace resource
} // namespace xc

#endif // _XC_REGISTRY_CONFIG_H_
/* end of config.h */

