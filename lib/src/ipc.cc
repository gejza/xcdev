/*
   File name:  ipc.cc
   Date:       2011/02/17 00:40
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xc/ipc.h"
#include "xc/error.h"

xc::shpool_t::shpool_t(size_t maxsize)
    : _owner(::getpid()) 
{
    XC_DBG("MM_create(%lu, 0)", maxsize);
    ::MM_create(maxsize, 0);
    //TODO MM_error
}
    
xc::shpool_t::~shpool_t()
{
    if (_owner == ::getpid()) {
        XC_DBG("::MM_destroy()");
        ::MM_destroy();
    }
}

void* xc::shpool_t::alloc(size_t size, bool zero)
{
    void* ptr = ::MM_malloc(size);
    if (zero) {
        ::memset(ptr, 0, size);
    }
    XC_DBG("Allocated %lu bytes. available %lu", size, this->available());
    return ptr;
}

void xc::shpool_t::free(void* ptr)
{
    ::MM_free(ptr);
    XC_DBG("Free bytes. available %lu", this->available());
}

size_t xc::shpool_t::available() const
{
    return ::MM_available();
}

