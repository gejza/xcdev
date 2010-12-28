/*
   File name:  lookup.h
   Date:       2010/12/13 22:14
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_REGISTRY_LOOKUP_H_
#define _XC_REGISTRY_LOOKUP_H_
#pragma once

#include <xc/data.h>

namespace xc {
namespace resource {

/**
 * Lookup class
 */
class Lookup_t
{
public:
    Lookup_t() {}
    virtual ~Lookup_t() {}

    virtual bool exist(const xc::data_t& key) const = 0;
    
    virtual bool lookup(const xc::data_t& key, xc::data_t& value) const  = 0;
};

} // namespace resource
} // namespace xc

#endif // _XC_REGISTRY_LOOKUP_H_
/* end of lookup.h */

