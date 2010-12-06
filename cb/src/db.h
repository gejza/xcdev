/*
   File name:  db.h.h
   Date:       2010/12/05 21:49
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _DB_H_
#define _DB_H_
#pragma once

#include <string.h>
#include <stdint.h>
#include <stddef.h>

struct Value_t
{
    const void* ptr;
    size_t size;

    Value_t() {}
    Value_t(const char* str)
        : ptr(str), size(::strlen(str)) {}
    template<typename Struct_t>
    Value_t(const Struct_t& o)
        : ptr(&o), size(sizeof(Struct_t)) {}
};

class DB_t
{
public:
    DB_t() {}
    virtual ~DB_t() {}
    
    virtual std::string get_string(const Value_t& key) = 0;

    virtual void dump() = 0;
};

class DBMake_t
{
public:
    DBMake_t() {}
    virtual ~DBMake_t() {}

    /*template<typename Key_t, typename Value_t>
    void add(const Key_t& key, const Value_t& value) {
        this->add(key.ptr(), key.size(), value.key(), value.size());
    }*/
    void add(const Value_t& key, const Value_t& val) {
        this->add(key.ptr, key.size, val.ptr, val.size);
    }
    virtual void add(const void* key, size_t klen, const void* val, size_t vlen) = 0;
};

#endif // _DB_H_
/* end of db.h.h */

