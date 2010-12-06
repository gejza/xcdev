/*
   File name:  cbdef.h
   Date:       2010/12/05 23:26
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_CBDEF_H_
#define _XC_CBDEF_H_
#pragma once

#include <vector>

namespace xc {

typedef uint32_t Id_t;
typedef uint32_t ClassId_t;
typedef uint32_t StrId_t;
typedef uint32_t LangId_t;

enum Lang_t
{
    MULTI = 0,
    EN = 1,
    CS = 2,
    SK = 3,
};

struct Callback_t
{
};

struct Template_t
{
    std::string id;
};

struct Menu_t
{
    struct Item_t
    {
        std::string uri;
        std::string title;
        std::vector<Item_t> submenu;
    };
    typedef std::vector<Item_t> Items_t;

    std::string id;
    Items_t items;
};


} // namespace xc

#endif // _XC_CBDEF_H_
/* end of cbdef.h */

