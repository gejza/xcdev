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

#include "include/xc/rd/out_cdb.h"

//////////////////////////////////
xc::rd::CDBMake_t::CDBMake_t(const char* fn)
    : _fd(fn, O_RDWR|O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
{
    if (::cdb_make_start(&this->_db, this->_fd.fd()) < 0) {
        ERROR(xc::error_t, "cdb_make_start: %s", ::strerror(errno));
    }
}

xc::rd::CDBMake_t::~CDBMake_t()
{
    if (::cdb_make_finish(&this->_db) < 0) {
        ERROR(xc::error_t, "cdb_make_finish: %s", ::strerror(errno));
    }
}

//TODO: to lib
template<typename Data_t>
xc::string human(const Data_t& data, size_t limit)
{
	return xc::human(data.data(), data.size(), limit);
}

void xc::rd::CDBMake_t::insert(const xc::rd::ns_t ns, const xc::data_t& key,
			const xc::data_t& value)
{
	XC_DBG("Insert ns=%d key=%s data=%s",
			ns, ::human(key, 20).c_str(), ::human(value, 60).c_str());

	xc::buffer_t k;
	k << ns << key;
	xc::buffer_t v;
	v << value << '\0';

    if (::cdb_make_add(&this->_db, k.data(), k.size(), v.data(), v.size()) < 0) {
        ERROR(xc::error_t, "cdb_make_add: %s", ::strerror(errno));
    }
}


