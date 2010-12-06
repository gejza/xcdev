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


} // namespace xc

#endif // _XC_CBDEF_H_
/* end of cbdef.h */

