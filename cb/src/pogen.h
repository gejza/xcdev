/*
   File name:  pogen.h
   Date:       2010/12/06 01:44
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_POGEN_H_
#define _XC_POGEN_H_
#pragma once

#include <stdio.h>
#include <errno.h>
#include <error.h>

namespace xc {

class POGen_t
{
public:
    POGen_t(const char* fn);
    virtual ~POGen_t();

    void add(const char* msg);
private:
    FILE* _file;
};


} // namespace xc

#endif // _XC_POGEN_H_
/* end of pogen.h */

