/*
   File name:  pool.cc
   Date:       2010/03/24 21:25
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#define XC_DEBUG_IDENT xc::debug::MEMORY

#include <stdlib.h>
#include <string.h>
#include <new>

#include "xc/pool.h"
#include "xc/log.h"
#include "xc/debug.h"

struct xc::pool_t::_chunk_t
{
    void* block;
    size_t freesize;
    _chunk_t* next;

    _chunk_t(void* block, size_t size)
        : block(block), freesize(size), next(0x0) {}
    void* alloc(size_t size) {
        if (size > freesize)
            return 0x0;
        void* ret = block;
        block = reinterpret_cast<char*>(block) + size;
        freesize -= size;
        return ret;
    }
};

xc::pool_t::pool_t() : _ch(0x0), _used(0), _alloc(0)
{
}

xc::pool_t::~pool_t()
{
    this->free();
}

void* xc::pool_t::alloc(size_t size)
{
    void* p = 0x0;
    if (_ch && (p = _ch->alloc(size))) {
        _used += size;
        return p;
    }
    _chunk_t* ch = _new_chunk(size + 1024);
    ch->next = _ch;
    _ch = ch;
    _used += size;
    return _ch->alloc(size);
}

void xc::pool_t::free()
{
    while (_ch) {
        _chunk_t* t = _ch;
        _ch = _ch->next;
        ::free(t);
        XC_DBG("Free chunk %p", t);
    }
    _used = 0;
    _alloc = 0;
}

xc::pool_t::_chunk_t* xc::pool_t::_new_chunk(size_t initsize)
{
    XC_DBG("Create new chunk of size %zd for pool %p", initsize, this);
    void* p = ::malloc(sizeof(_chunk_t) + initsize);
    _alloc += sizeof(_chunk_t) + initsize;
    return new (p) _chunk_t(reinterpret_cast<_chunk_t*>(p)+1, initsize);
}

void* xc::pool_t::copy(const void* src, size_t size)
{
    void* dest = this->alloc(size + 1); // +1 security
    ::memcpy(dest, src, size);
    return dest;
}

const char* xc::pool_t::strdup(const char* str, size_t len/*0*/)
{
    if (!len)
        len = ::strlen(str);
    char* dest = reinterpret_cast<char*>(this->copy(str, len));
    dest[len] = '\0';
    return dest;
}

