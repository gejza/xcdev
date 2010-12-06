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

namespace xc {

class POGen_t;

class CBMake_t
{
public:
    CBMake_t();
    virtual ~CBMake_t();

    void set_default(Lang_t lang) {
        this->_def_lang = lang;
    }
    void generate_pot(const char* fn);
    void import_xml(const char* fn);

    // string
    StrId_t string(const char* str) {
        return this->string(this->_def_lang, str);
    }
    StrId_t string(Lang_t lang, const char* str);
    void string(const StrId_t id, Lang_t lang, const char* str);

    // hook
    void hook(const char* name);

    // alias
    void alias(const char* route, Lang_t lang, const char* alias);
    // debug
    const char* get_lang(Lang_t lang);
protected:
    Id_t seq()
    {
        return ++_curid;
    }

private:
    Id_t _curid;
    Lang_t _def_lang;
    DBMake_t* _db;
    POGen_t* _po;
};

} // namespace xc

#endif // _XC_CBMAKE_H_
/* end of cbmake.h */

