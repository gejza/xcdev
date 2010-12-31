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
#include "StdAfx.h"

#include <iostream>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <memory.h>
#include <stdlib.h>

#include <xc/buffer.h>

#include "../include/xc/rd/cdb.h"

///////////////////////////////////////////    
xc::rd::ConstDB_t::ConstDB_t(const char* fn)
    : _fd(fn)
{
    int ret = cdb_init(&this->_db, this->_fd.fd());
}

xc::rd::ConstDB_t::~ConstDB_t()
{
}

bool xc::rd::ConstDB_t::exist(const xc::rd::ns_t ns, const xc::data_t& key) const
{
	xc::buffer_t k;
	k << ns << key;
    if (cdb_find(const_cast<cdb*>(&this->_db), k.data(), k.size()) > 0)
    {
        return true;
    }
    return false;
}

bool xc::rd::ConstDB_t::lookup(const xc::rd::ns_t ns, const xc::data_t& key,
		xc::data_t& val) const
{
	xc::buffer_t k;
	k << ns << key;
    if (cdb_find(const_cast<cdb*>(&this->_db), k.data(), k.size()) > 0)
    {
        uint32_t vpos = cdb_datapos(&this->_db);
        uint32_t vlen = cdb_datalen(&this->_db);
        val = xc::data_t(reinterpret_cast<const uint8_t*>
				(cdb_get(const_cast<cdb*>(&this->_db), vlen, vpos)), vlen - 1);
        return true;
    }
    return false;
}

/*std::string xc::rd::ConstDB_t::get_string(const Value_t& key)
{
    Value_t val;
    if (this->get(key, val))
    {
        std::string str;
        str.assign((const char*)val.ptr, val.size);
        return str;
    }
    return "";
}*/

void xc::rd::ConstDB_t::dump()
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

