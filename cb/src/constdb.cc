/*
   File name:  cdb.cc
   Date:       2010/12/05 22:11
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <iostream>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <memory.h>
#include <stdlib.h>
#include "constdb.h"

///////////////////////////////////////////    
ConstDB_t::ConstDB_t(const char* fn)
    : _fd(fn)
{
    int ret = cdb_init(&this->_db, this->_fd.fd());
}

ConstDB_t::~ConstDB_t()
{
}

bool ConstDB_t::get(const Value_t& key, Value_t& val)
{
    if (cdb_find(&this->_db, key.ptr, key.size) > 0)
    {
        uint32_t vpos = cdb_datapos(&this->_db);
        uint32_t vlen = cdb_datalen(&this->_db);
        val.ptr = cdb_get(&this->_db, vlen, vpos);
        val.size = vlen;
        return true;
    }
    return false;
}

std::string ConstDB_t::get_string(const Value_t& key)
{
    Value_t val;
    if (this->get(key, val))
    {
        std::string str;
        str.assign((const char*)val.ptr, val.size);
        return str;
    }
    return "";
}

void ConstDB_t::dump()
{
    /*struct cdb_find dbf;
    cdb_findinit(&dbf, &_db, "", 0);
    while(cdb_findnext(&dbf) > 0) {
        uint32_t vpos = cdb_datapos(&dbf);
        uint32_t vlen = cdb_datalen(&dbf);
        std::cout << vpos << ":" << vlen << std::endl;
    }
    */
    int ret;
    unsigned pos;
    cdb_seqinit(&pos, &_db);
    while ((ret = cdb_seqnext(&pos, &_db)) > 0)
    {

        const void* key = cdb_getkey(&_db);
        uint32_t klen = cdb_keylen(&_db);
        const void* val = cdb_getdata(&_db);
        uint32_t vlen = cdb_datalen(&_db);
        //std::cout << kpos << ":" << klen << "|" << vpos << ":" << vlen << std::endl;
    }
    if (ret < 0) {

        std::cout << "error" << std::endl;
    }
}

//////////////////////////////////
ConstDBMake_t::ConstDBMake_t(const char* fn)
    : _fd(fn, O_RDWR|O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
{
    int ret = cdb_make_start(&this->_db, this->_fd.fd());
    //std::cout << "Start cdb: " << ret << std::endl;
}

ConstDBMake_t::~ConstDBMake_t()
{
    int ret = cdb_make_finish(&this->_db);
    //std::cout << "Finish cdb: " << ret << std::endl;
}

void ConstDBMake_t::add(uint32_t table, const void* key, size_t klen,
                        const void* val, size_t vlen)
{
    int ret = cdb_make_add(&this->_db, key, klen, val, vlen);
    //std::cout << "Key: " << klen << " Value: " << vlen << " ret:" << ret << std::endl;
}
