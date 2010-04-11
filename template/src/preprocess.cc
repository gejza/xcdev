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


const xc::templ::scanner_t::term_t xc::templ::preproc_t::read()
{
    _buffer.clear();

    scanner_t::term_t t(_last ? _last : _scanner.read());
    // reset last token
    _last.type = 0;

    // check end
    if (!t)
        return t;

    switch (t.type) {
    case TI_TEXT:
        break;
    case TI_RAW:
        break;
    case TI_END:
        return t;
    default:
        return t;
    }

    // move to buffer
    _buffer.assign(t.text, t.length);

    while ((_last  = _scanner.read()) && t.type == _last.type) {

        // join text
        XC_DBG("Join with next token '%s'", xc::human(_last.text, 80).c_str());
        _buffer.append(_last.text, _last.length);
    }

    t.text = _buffer.c_str();
    t.length = _buffer.size();

    return t;
}





