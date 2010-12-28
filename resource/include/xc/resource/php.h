/*
   File name:  php.h
   Date:       2010/12/18 10:31
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_REGISTRY_PHP_H_
#define _XC_REGISTRY_PHP_H_
#pragma once

#include "env.h"

namespace xc {
namespace resource {

class PHP_t : public Env_t
{
public:
    PHP_t();
    virtual ~PHP_t();
};

} // namespace resource
} // namespace xc

#endif // _XC_REGISTRY_PHP_H_
/* end of php.h */

