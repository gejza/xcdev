/*
   File name:  makecdb.h
   Date:       2010/12/18 15:11
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _MAKECDB_H_
#define _MAKECDB_H_
#pragma once

#include <xc/fd.h>
#include <cdb.h>

#include "db.h"

class ConstDBMake_t : public DBMake_t
{
public:
    ConstDBMake_t(const char* fn);
    virtual ~ConstDBMake_t();
    virtual void add(uint32_t table, const void*, size_t, const void*, size_t);
private:
    xc::fd_t _fd;
    struct cdb_make _db;
};

#endif // _MAKECDB_H_
/* end of makecdb.h */

