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
#include <xc/data.h>

#include "../include/xc/rd/cdb.h"

////////////////////////////////////////////
xc::rd::ConstDB_t::Cursor_t::Cursor_t(struct cdb* db)
    : _db(db), _ctx(0)
{
    cdb_seqinit(&_ctx, this->_db);
    /*while ((ret = ::cdb_seqnext(&pos, this->_get())) > 0)
    {
        xc::rd::ns_t ns;
        xc::data_t k(reinterpret_cast<const uint8_t*>(
				cdb_getkey(const_cast<cdb*>(&this->_db))), cdb_keylen(const_cast<cdb*>(&this->_db)));
        k >> ns;
        const xc::data_t v(reinterpret_cast<const uint8_t*>(
				cdb_getdata(const_cast<cdb*>(&this->_db))), cdb_datalen(const_cast<cdb*>(&this->_db)));
        iter(ns, k, v);
    }*/
}

void xc::rd::ConstDB_t::Cursor_t::next()
{
    int ret = ::cdb_seqnext(&_ctx, this->_get());
    if (ret <= 0) {
        _db = NULL;
    }
}

xc::rd::ns_t xc::rd::ConstDB_t::Cursor_t::ns() const
{
    return *reinterpret_cast<const xc::rd::ns_t*>(cdb_getkey(this->_get()));
}

const xc::data_t xc::rd::ConstDB_t::Cursor_t::key() const
{
    return xc::data_t(reinterpret_cast<const uint8_t*>(cdb_getkey(this->_get())),
                        cdb_keylen(this->_get())) += sizeof(xc::rd::ns_t);
}

const xc::data_t xc::rd::ConstDB_t::Cursor_t::value() const
{
    return xc::data_t(reinterpret_cast<const uint8_t*>(cdb_getdata(this->_get())),
            cdb_datalen(this->_get()));
}

///////////////////////////////////////////    
xc::rd::ConstDB_t::ConstDB_t(const char* fn)
    : _fd(fn)
{
    cdb_init(&this->_db, this->_fd.fd());
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

xc::rd::ConstDB_t::Cursor_t xc::rd::ConstDB_t::cursor() const
{
    return xc::rd::ConstDB_t::Cursor_t(this->_get());
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

void xc::rd::ConstDB_t::dump(Iterator_t iter) const
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
    cdb_seqinit(&pos, const_cast<cdb*>(&this->_db));
    while ((ret = ::cdb_seqnext(&pos, this->_get())) > 0)
    {
        xc::rd::ns_t ns = 0;
        xc::data_t k(reinterpret_cast<const uint8_t*>(
				cdb_getkey(const_cast<cdb*>(&this->_db))), cdb_keylen(const_cast<cdb*>(&this->_db)));
        k >> ns;
        const xc::data_t v(reinterpret_cast<const uint8_t*>(
				cdb_getdata(const_cast<cdb*>(&this->_db))), cdb_datalen(const_cast<cdb*>(&this->_db)));
        iter(ns, k, v);
    }

    if (ret < 0) {

        std::cout << "error" << std::endl;
    }
}

