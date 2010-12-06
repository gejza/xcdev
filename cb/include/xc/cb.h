/*
   File name:  cb.h
   Date:       2010/12/05 23:23
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_CB_H_
#define _XC_CB_H_
#pragma once

#include <string>

#include "cbdef.h"

class DB_t;

namespace xc {

class CB_t
{
public:
    CB_t();
    virtual ~CB_t();

    // string
    std::string string(const StrId_t id, Lang_t lang);
    // hooks
    // alias
    // template
private:
    DB_t* _db;
};

} // namespace xc

#endif // _XC_CB_H_
/* end of cb.h */

