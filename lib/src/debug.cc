/*
   File name:  debug.cc
   Date:       2010/03/24 22:19
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <set>
#include <string.h>

#include "xc/debug.h"
#include "xc/string.h"
#include "xc/text.h"
#include "xc/log.h"

namespace {

    struct debug_mask_t
    {
        bool en_all;
        bool en_regex;
        bool en_ident;
        xc::regex_t regex;
        std::set<unsigned long> ident;

        debug_mask_t() : en_all(false), en_regex(false), en_ident(false) {
        }

        bool test(unsigned long idt, const xc::debug::loc_t& loc) const {
            if (en_all)
                return true;
            if (en_ident && ident.find(idt) != ident.end())
                return true;
            if (en_regex && regex == loc.filename())
                return true;
            return false;
        }

        void enable() {
            en_all = true;
        }

        void disable() {
            en_all = false;
            en_regex = false;
            en_ident = false;
        }

        void enable(const char* ex) {
            regex = ex;
            en_regex = true;
        }

        void enable(unsigned long idt) {
            ident.insert(idt);
            en_ident = true;
        }
    };

    debug_mask_t _mask;
}

xc::debug::trace_t::trace_t(const trace_t& other)
    : _p(0x0), _size(0)
{
    _size = other._size;
    if (_size) {
        _p = new void*[_size];
        memcpy(_p, other._p, sizeof(void*) * _size);
    }
}


xc::debug::trace_t::~trace_t()
{
    if (_p)
        delete [] _p;
}


xc::debug::trace_t& xc::debug::trace_t::operator=(const trace_t& other)
{
    if (this != &other) {  // no self-assignmet
        // deep copy
        if (_p)
            delete [] _p;
        _p = 0x0;
        _size = other._size;
        if (_size) {
            _p = new void*[_size];
            ::memcpy(_p, other._p, sizeof(void*) * _size);
        }
    }
    return *this;
}

xc::string xc::debug::trace_t::name(unsigned int index) const
{
    if (index < 0 || index >= _size)
        return "";
    return symbol_name(_p[index]);
}

static char ws[] = "                                                                      ";

void xc::debug::trace_t::dump(FILE* out, size_t indent) const
{
    static trace_t last(false);
    size_t max = *this - last;
    for (unsigned int i=0; i < max; i++) {
        fprintf(out, "\n%.*s%s", (int)indent, ws, this->name(i).c_str());
        
    }
    if (max)
        fprintf(out, "\n%.*s", (int)indent, ws);
    //::backtrace_symbols_fd(this->_p, this->_size, fd);
    last = *this;
}

void xc::debug::debug_enable()
{
    _mask.enable();
}

void xc::debug::debug_enable(unsigned long ident)
{
    _mask.enable(ident);
    fprintf(stdout, "Enable debugging for %s\n", ident_name(ident));
}

void xc::debug::debug_enable(const char* regex)
{
    _mask.enable(regex);
}

void xc::debug::debug_disable()
{
    _mask.disable();
}

void xc::debug::print(unsigned long ident, const loc_t& loc, const char* format, ...)
{
    if (!_mask.test(ident, loc))
        return;
    va_list arg;
    va_start(arg, format);
    {
        xc::colored_t c(stdout, 30);
        fprintf(stdout, "%s(%s:%d): ", ident_name(ident), loc.filename(), loc.line());
        vfprintf(stdout, gettext(format), arg);
    }
    fprintf(stdout, "\n");
    va_end(arg);
}

void xc::debug::trace(unsigned long ident, const loc_t& loc, const char* format, ...)
{
    if (!_mask.test(ident, loc))
        return;
    va_list arg;
    va_start(arg, format);
    {
        xc::colored_t c(stdout, 30);
        fprintf(stdout, "TRACE(%s): ", loc.filename(), loc.line());
        vfprintf(stdout, gettext(format), arg);
    }
    fprintf(stdout, "\n");
    va_end(arg);
}

const char* xc::debug::ident_name(unsigned long idn)
{
    switch (idn) {
#define CASE(n) case n: return #n;
    case 0: return "DEFAULT";
    CASE(MEMORY)
    CASE(FS)
    CASE(NET)
    CASE(CONFIG)
    CASE(LOG)
    CASE(USER)
    default:
        return "";
    };
}


