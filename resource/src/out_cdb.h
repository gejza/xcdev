/*
   File name:  out_cdb.h
   Date:       2010/12/18 15:11
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _OUT_CDB_H_
#define _OUT_CDB_H_
#pragma once

#include <xc/fd.h>
#include <cdb.h>

#include "make.h"

class ConstDBMake_t : public Output_t
{
public:
    ConstDBMake_t(const char* fn);
    virtual ~ConstDBMake_t();
    virtual void add(uint32_t table, const void*, size_t, const void*, size_t);
private:
    xc::fd_t _fd;
    struct cdb_make _db;
};

#endif // _OUT_CDB_H_
/* end of out_cdb.h */

