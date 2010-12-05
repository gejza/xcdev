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

namespace {

struct StrKey_t
{
    xc::StrId_t id;
    xc::LangId_t lang;

    StrKey_t(xc::StrId_t _id, xc::LangId_t _lang)
        : id(_id), lang(_lang) {}
};

} // namespace

#endif // _XC_DBOBJ_H_
/* end of dbobj.h */

