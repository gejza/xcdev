/*
   File name:  bytecode.cc
   Date:       2010/03/26 11:30
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xc/template/bytecode.h"

const char* xc::templ::ti_name(xc::templ::inscode_t term)
{
    switch (term) {
#define T(name) case TI_##name: return #name;
    T(END)
    T(RAW)
    T(TEXT)
    
    T(VALUE)
    T(DICT)
    T(DICT_VALUE)
    
    T(PAGE)
    T(PAGE_LIST)

    T(FRAG)

    T(DEBUG)

    T(FRAG_BEGIN)
    T(FRAG_END)

    T(SECTION)
    T(INCLUDE)
#undef T
    default:
        return "UNKNOWN";
    };
}


