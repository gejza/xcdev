/*
   File name:  refobj.cc
   Date:       2010/03/30 14:40
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xc/refobj.h"

xc::refobj_t::refobj_t()
    : _refcount(1)
{
}

xc::refobj_t::~refobj_t()
{
}



