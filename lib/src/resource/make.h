/*
   File name:  cbmake.h
   Date:       2010/12/05 21:07
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_CBMAKE_H_
#define _XC_CBMAKE_H_
#pragma once

#include <memory>
#include "cbdef.h"

class DBMake_t;


enum TableId_t
{
    NS_TABLE = 2,
    STR_TABLE = 23,
};

using namespace xc;

class Output_t
{
public:
    virtual ~Output_t() {}
    
    virtual void add(uint32_t table, const void*, size_t, const void*, size_t) = 0;
};

class Make_t
{
public:
    Make_t(Output_t& out);
    virtual ~Make_t();

    void set_default(Lang_t lang) {
        this->_def_lang = lang;
    }

    // string
    StrId_t string(const char* str) {
        return this->string(this->_def_lang, str);
    }
    StrId_t string(Lang_t lang, const char* str);
    void string(const StrId_t id, Lang_t lang, const char* str);
    void string(const std::string& msgid, const std::string& msgstr, Lang_t lang);

    // hook
    //void hook(const char* name, const Callback_t& cb);

    // alias
    void alias(const char* route, Lang_t lang, const char* alias);

    Id_t add(const Menu_t& menu);
    void add(const Template_t& templ);
    // debug
    const char* get_lang(Lang_t lang);
protected:
    Id_t seq()
    {
        return ++_curid;
    }

private:
    Output_t& _out;
    Lang_t _def_lang;
    Id_t _curid;
};

#endif // _XC_CBMAKE_H_
/* end of cbmake.h */

