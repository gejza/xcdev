/*
   File name:  constdb.h.h
   Date:       2010/12/05 21:49
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _CONSTDB_H_
#define _CONSTDB_H_
#pragma once

#include <cdb.h>
#include "db.h"
#include "fd.h"

class ConstDB_t : public DB_t
{
public:
    ConstDB_t(const char* fn);
    virtual ~ConstDB_t();
    virtual bool get(const Value_t& key, Value_t& val);
    virtual std::string get_string(const Value_t& key);
    
    virtual void dump();
private:
    FD_t _fd;
    struct cdb _db;
};

class ConstDBMake_t : public DBMake_t
{
public:
    ConstDBMake_t(const char* fn);
    virtual ~ConstDBMake_t();
    virtual void add(uint32_t table, const void*, size_t, const void*, size_t);
private:
    FD_t _fd;
    struct cdb_make _db;
};


#endif // _CONSTDB_H_
/* end of constdb.h.h */

