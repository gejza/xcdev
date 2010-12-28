/*
   File name:  out_debug.h
   Date:       2010/12/20 02:14
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _OUT_DEBUG_H_
#define _OUT_DEBUG_H_
#pragma once

#include "make.h"

class DebugOutput_t : public Output_t
{
public:
    virtual void add(uint32_t table, const void*, size_t klen, const void*, size_t vlen)
    {
        printf("ADD %x: %ld -> %ld\n", table, klen, vlen);
    }
};


#endif // _OUT_DEBUG_H_
/* end of out_debug.h */

