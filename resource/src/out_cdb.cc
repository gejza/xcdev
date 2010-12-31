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
    int ret = cdb_make_start(&this->_db, this->_fd.fd());
    //std::cout << "Start cdb: " << ret << std::endl;
}

xc::rd::CDBMake_t::~CDBMake_t()
{
    int ret = cdb_make_finish(&this->_db);
    //std::cout << "Finish cdb: " << ret << std::endl;
}

void xc::rd::CDBMake_t::insert(const xc::rd::ns_t ns, const xc::data_t& key,
			const xc::data_t& value)
{
	xc::buffer_t k;
	k << ns << key;
	xc::buffer_t v;
	v << value << '\0';
    int ret = cdb_make_add(&this->_db, k.data(), k.size(), v.data(), v.size());
    //std::cout << "Key: " << klen << " Value: " << vlen << " ret:" << ret << std::endl;
}


