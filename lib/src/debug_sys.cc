/*
   File name:  debug_sys.cc
   Date:       2010/03/25 06:18
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>
#include <execinfo.h>
#include <demangle.h>
#include <dlfcn.h>

#include "xc/debug.h"
#include "xc/string.h"
#include "xc/text.h"
#include "xc/log.h"

xc::string xc::debug::demangle(const char* name)
{
    char* dn = cplus_demangle(name, DMGL_ANSI|DMGL_PARAMS);
    if (!dn)
        return name ? name : "";
    xc::string ret(dn);
    ::free(dn);
    return ret;
}

xc::string xc::debug::symbol_name(const void* p)
{
    Dl_info info;
    if (!dladdr(p, &info))
        return xc::format("[%p]", p);

    if (!info.dli_sname)
        return xc::format("%s:%p", info.dli_fname, p);

    return xc::format("%s:%s", info.dli_fname, demangle(info.dli_sname).c_str());
}

xc::debug::trace_t::trace_t(bool trace)
    : _p(0x0), _size(0)
{
    if (trace) {
        void* arr[20];
        size_t s = ::backtrace(arr, 20);
        if (s > 1) {
            _p = new void*[s - 1];
            while (--s) {
                _p[_size++] = arr[s];
            }
        }
    }
}

xc::debug::loc_src_t::loc_src_t(const void* p)
{
    Dl_info info;
    if (!dladdr(p, &info)) {
        _filename = "";
        _func = "";
        return;
    }
    _filename = info.dli_fname;
    _line = 0;
    _func =info.dli_sname;
}


