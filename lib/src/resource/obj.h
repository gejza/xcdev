/*
   File name:  dbobj.h
   Date:       2010/12/05 21:45
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_DBOBJ_H_
#define _XC_DBOBJ_H_
#pragma once

#include "cbdef.h"

namespace {

struct CBInfo_t
{
    uint32_t api; ///< API version
};

enum {
    OBJ_STRING = 0x10,
};

struct CBKey_t
{
    xc::ClassId_t classid;
    CBKey_t(xc::ClassId_t _classid)
        : classid(_classid) {}
};

struct StrKey_t : CBKey_t
{
    xc::StrId_t id;
    xc::LangId_t lang;

    StrKey_t(xc::StrId_t _id, xc::LangId_t _lang)
        : CBKey_t(OBJ_STRING), id(_id), lang(_lang) {}
};


struct CallBack_t
{

};

struct MenuItem_t
{
    xc::StrId_t title;
};

} // namespace

#endif // _XC_DBOBJ_H_
/* end of dbobj.h */

