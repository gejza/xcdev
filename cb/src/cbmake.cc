/*
   File name:  cbmake.cc
   Date:       2010/12/05 21:00
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdint.h>

#include <iostream>
/*#include <xc/log.h>
#include <xc/error.h>
*/
#include "../include/xc/cbmake.h"
#include "dbobj.h"
#include "constdb.h"

//////////////////////////////////
xc::CBMake_t::CBMake_t()
    : _curid(0), _def_lang(xc::MULTI), _db(NULL)
{
    this->_db = new ConstDBMake_t("test.cb");
}

xc::CBMake_t::~CBMake_t()
{
    delete this->_db;
}

xc::StrId_t xc::CBMake_t::string(Lang_t lang, const char* str)
{
    StrId_t id = seq();
    this->string(id, lang, str);
    return id;
}

void xc::CBMake_t::string(const StrId_t id, Lang_t lang, const char* str)
{
    std::clog << "Set string " << id << " = (" << this->get_lang(lang) << ")" << str << std::endl;
    StrKey_t key(id, lang);
    this->_db->add(key, str);
}

const char* xc::CBMake_t::get_lang(Lang_t lang)
{
    switch (lang)
    {
    case xc::EN:
        return "en";
    case xc::CS:
        return "cs";
    case xc::SK:
        return "sk";
    default:
        return "";
    };
}
