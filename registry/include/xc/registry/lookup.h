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

namespace xc {
namespace registry {

/**
 * Lookup class
 */
class Lookup_t
{
public:
    Lookup_t() {}
    virtual ~Lookup_t() {}

    virtual bool exist() const = 0;
    
    virtual const char* get(const char*) const  = 0;
};

} // namespace registry
} // namespace xc

#endif // _XC_REGISTRY_LOOKUP_H_
/* end of lookup.h */

