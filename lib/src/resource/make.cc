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

#include <xc/log.h>
#include <xc/error.h>
#include <xc/serialize.h>
#include <xc/unserialize.h>

#include <stdint.h>

#include <iostream>
#include "obj.h"
#include "include/xc/rd/out_cdb.h"
#include "make.h"

//////////////////////////////////
Make_t::Make_t(Output_t& out)
    : _out(out), _def_lang(xc::MULTI), _curid(0)
{
}

Make_t::~Make_t()
{
}

xc::StrId_t Make_t::string(Lang_t lang, const char* str)
{
    StrId_t id = seq();
    this->string(id, lang, str);
    return id;
}

void Make_t::string(const StrId_t id, Lang_t lang, const char* /*str*/)
{
    StrKey_t key(id, lang);
    //DBTableMake_t db(*this->_db, 12);
    //this->_db->add(key, str);
    //db.add(key, str);
    //_out.add(12, );
}
    
void Make_t::string(const std::string& msgid, const std::string& msgstr, Lang_t lang)
{
    xc::StrId_t id = this->string(lang, msgstr.c_str());
    this->_out.add(STR_TABLE, msgid.data(), msgid.size(), &id, sizeof(id));
}

const char* Make_t::get_lang(Lang_t lang)
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

//////////////////////////////////////////
/*void Make_t::hook(const char* name, const Callback_t& cb)
{
    //HookKey_t key(name);
    // table hooks
}*/

void Make_t::alias(const char* /*route*/, Lang_t /*lang*/, const char* /*alias*/)
{
    // table alias:  route-lang => alias
    // table route:  alias -> route
}

xc::Id_t Make_t::add(const Menu_t& menu)
{
    std::cout << menu.id << std::endl;
    for (Menu_t::Items_t::const_iterator i=menu.items.begin();i != menu.items.end(); i++)
    {
        this->string(i->title.c_str());
        std::cout << i->title << ":" << i->uri << std::endl;
    }
    return seq();
}

