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
#include "../include/xc/cbmake.h"
#include "obj.h"
#include "constdb.h"
#include "pogen.h"
#include "xmlparser.h"

//////////////////////////////////
xc::CBMake_t::CBMake_t()
    : _curid(0), _def_lang(xc::MULTI), _db(NULL), _po(NULL)
{
    this->_db = new ConstDBMake_t("test.cb");
}

xc::CBMake_t::~CBMake_t()
{
    delete this->_db;
    if (this->_po)
        delete _po;
}

void xc::CBMake_t::generate_pot(const char* fn)
{
    //TODO
    _po = new POGen_t(fn);
}
    
void xc::CBMake_t::load_xml(const char* fn)
{
    XMLParser_t xml(*this);
    xml.parse(fn);
}

xc::StrId_t xc::CBMake_t::string(Lang_t lang, const char* str)
{
    StrId_t id = seq();
    this->string(id, lang, str);
    if (_po) {
        _po->add(str);
    }
    return id;
}

void xc::CBMake_t::string(const StrId_t id, Lang_t lang, const char* str)
{
    StrKey_t key(id, lang);
    DBTableMake_t db(*this->_db, 12);
    //this->_db->add(key, str);
    db.add(key, str);
}
    
void xc::CBMake_t::string(const std::string& msgid, const std::string& msgstr, Lang_t lang)
{
    xc::StrId_t id = this->string(lang, msgstr.c_str());
    this->_db->add(STR_TABLE, msgid, id);
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

//////////////////////////////////////////
/*void xc::CBMake_t::hook(const char* name, const Callback_t& cb)
{
    //HookKey_t key(name);
    // table hooks
}*/

void xc::CBMake_t::alias(const char* route, Lang_t lang, const char* alias)
{
    // table alias:  route-lang => alias
    // table route:  alias -> route
}

const xc::Id_t xc::CBMake_t::add(const Menu_t& menu)
{
    std::cout << menu.id << std::endl;
    for (Menu_t::Items_t::const_iterator i=menu.items.begin();i != menu.items.end(); i++)
    {
        this->string(i->title.c_str());
        std::cout << i->title << ":" << i->uri << std::endl;
    }
    return seq();
}

xc::serialize_t& operator<<(xc::serialize_t& out, const xc::Callback_t& cb)
{
    out.write("include", cb.include);
    out.write("script", cb.script);
    out.write("class", cb.cls);
    out.write("method", cb.method);
    return out;
}

xc::serialize_t& operator<<(xc::serialize_t& out, const xc::Template_t& templ)
{
    out << templ.call;
    return out;
}

void xc::CBMake_t::add(const Template_t& templ)
{
    xc::serial_t out;
    out << templ.call;

    xc::unserialize_t ser(out.str());
    dump(ser);
    std::cout << "Template " << templ.id << std::endl;
    //std::cout << "Script " << templ.call.script << std::endl;

    StrKey_t key(1, xc::CS);
    this->_db->add(STR_TABLE, key, out.str());

    //return seq();
}
