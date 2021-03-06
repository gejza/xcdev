/*
   File name:  preprocess.cc
   Date:       2010/03/26 21:10
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <xc/log.h>
#include <xc/error.h>
#include <xc/text.h>
#include "xc/template/preprocess.h"

void xc::templ::preproc_t::set(const char* name)
{
    LOG_INFO(1, "Read template '%s'", name);
    if (_curr)
        _stack.push(_curr);
    _curr = _files.open(name);
}

const xc::templ::scanner_t::term_t xc::templ::preproc_t::read()
{
retry:
    _buffer.clear();
    if (!_curr) {
        if (!_stack.empty()) {
            _curr = _stack.top();
            _stack.pop();
        } else {
            return _last;
        }
    }

    scanner_t::term_t t(_last ? _last : _curr->read());
    // reset last token
    _last.type = 0;

    switch (t.type) {
    case TI_TEXT:
        break;
    case TI_RAW:
        break;
    case TI_END:
        {
            if (_stack.empty())
                return _last;
            xc::templ::scanner_t::term_t dbg(TI_DEBUG);
            _buffer = xc::format("end %s", _curr->file_name());
            dbg.text = _buffer.c_str();
            dbg.length = _buffer.size();
            delete _curr;
            _curr = 0x0;
            return dbg;
        }
    case TI_INCLUDE:
        {
            xc::string param = t.get_param();
            set(param.c_str());
            xc::templ::scanner_t::term_t dbg(TI_DEBUG);
            _buffer = xc::format("begin %s", param.c_str());
            dbg.text = _buffer.c_str();
            dbg.length = _buffer.size();
            return dbg;
        }
    default:
        return t;
    }

    // move to buffer
    _buffer.assign(t.text, t.length);

    while ((_last  = _curr->read()) && t.type == _last.type) {

        // join text
        XC_DBG("Join with next token '%s'", xc::human(_last.text, 80).c_str());
        _buffer.append(_last.text, _last.length);
    }

    t.text = _buffer.c_str();
    t.length = _buffer.size();

    return t;
}





