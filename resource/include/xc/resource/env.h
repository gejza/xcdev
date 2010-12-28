/*
   File name:  env.h
   Date:       2010/12/15 21:44
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_REGISTRY_ENV_H_
#define _XC_REGISTRY_ENV_H_
#pragma once

#include <libconfig.h++>

namespace xc {
namespace resource {

class Lookup_t;

class Env_t
{
public:
    Env_t();
    virtual ~Env_t() {}

    const Lookup_t& get() const;
protected:
    libconfig::Config _config;
};

} // namespace resource
} // namespace xc

#endif // _XC_REGISTRY_ENV_H_
/* end of env.h */

