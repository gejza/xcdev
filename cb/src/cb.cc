/*
   File name:  registry.cc
   Date:       2010/12/03 14:05
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan.dunghubel@gmail.com>

   Copyright (C) 2010 Milan Dunghubel <milan.dunghubel@gmail.com>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdint.h>

#include <iostream>
/*#include <xc/log.h>
#include <xc/error.h>
*/
#include "../include/xc/cb.h"
#include "dbobj.h"
#include "constdb.h"

//////////////////////////////////
xc::CB_t::CB_t()
    : _db(NULL)
{
    this->_db = new ConstDB_t("test.cb");
}

xc::CB_t::~CB_t()
{
    delete this->_db;
}

std::string xc::CB_t::string(const StrId_t id, Lang_t lang)
{
    StrKey_t key(id, lang);
    std::string ret = this->_db->get_string(key);
    if (ret.empty()) {
        key.lang = xc::MULTI;
        ret = this->_db->get_string(key);
    }
    return ret;
}



