/*
   File name:  cdb.h
   Date:       2010/12/05 21:49
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_REGISTRY_CDB_H_
#define _XC_REGISTRY_CDB_H_
#pragma once

#include <xc/fd.h>
#include <xc/data.h>
#include <cdb.h>

#include "lookup.h"

namespace xc {
namespace registry {

class ConstDB_t : public Lookup_t
{
public:
    ConstDB_t(const char* fn);
    virtual ~ConstDB_t();
    
    virtual bool exist() const;
    
    virtual bool lookup(const xc::data_t& key, xc::data_t& value) const;
    
    virtual void dump();
private:
    xc::fd_t _fd;
    struct cdb _db;
};

} // namespace registry
} // namespace xc

#endif // _XC_REGISTRY_CDB_H_
/* end of cdb.h */

