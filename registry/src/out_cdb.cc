/*
   File name:  makecdb.cc
   Date:       2010/12/18 15:13
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "StdAfx.h"

#include "out_cdb.h"

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

std::string key_hash(uint32_t table, const void* data, size_t size)
{
    std::string key;
    key.append(reinterpret_cast<const char*>(&table), sizeof(table));
    key.append(reinterpret_cast<const char*>(data), size);
    return key;
}

void ConstDBMake_t::add(uint32_t table, const void* key, size_t klen,
                        const void* val, size_t vlen)
{
    std::string k(key_hash(table, key, klen));
    int ret = cdb_make_add(&this->_db, k.data(), k.size(), val, vlen);
    //std::cout << "Key: " << klen << " Value: " << vlen << " ret:" << ret << std::endl;
}


