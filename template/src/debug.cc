/*
   File name:  debug.cc
   Date:       2010/03/26 21:35
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <xc/log.h>
#include <xc/text.h>

#include "xc/template/error.h"
#include "xc/template/scanner.h"
#include "xc/template/preprocess.h"
#include "xc/template/debug.h"

namespace {

void output_color(FILE* out, const xc::templ::scanner_t::term_t& t)
{
    static int style = 1;
    int color = 0;
    switch (t.type) {
    case xc::templ::TI_RAW:
        color = 30;
        break;
    case xc::templ::TI_TEXT:
        color = 36;
        break;
    case xc::templ::TI_PAGE:
        color = 31;
        break;
    case xc::templ::TI_DICT:
        color = 32;
        break;
    case xc::templ::TI_VALUE:
        color = 33;
        break;
    case xc::templ::TI_FRAG_BEGIN:
        style = 4;
        color = 37;
        break;
    case xc::templ::TI_FRAG_END:
        style = 1;
        color = 37;
        break;
    default:
        {
            xc::colored_t c(stdout, 31, 7);
            printf("%s", t.name());
        }
        return;
    };

    if (!color) {
        fwrite(t.text, 1, t.length, stdout);
        return;
    }

    xc::colored_t c(out, color, style);
    fwrite(t.text, 1, t.length, stdout);
}

void output_desc(FILE* out, const xc::templ::scanner_t::term_t& t)
{
    fprintf(out, "%d: %s '%s' \n", t.line, t.name(), xc::human(t.text, 120).c_str());
}

void dump_raw(FILE* out, xc::templ::scanner_t& scan,
        xc::templ::dump_format_t format)
{
    void (*of)(FILE*, const xc::templ::scanner_t::term_t&);
    switch (format) {
    case xc::templ::DF_COLOR:
        of = output_color;
        break;
    case xc::templ::DF_LIST:
        of = output_desc;
        break;
    default:
        ERROR(xc::templ::error_t, "Unknown dump format type.");
        break;
    };

    while (xc::templ::scanner_t::term_t t = scan.read()) {
        of(out, t);
    }
}

} // namespace

void xc::templ::dump(FILE* out, xc::templ::scanner_t& scan,
        xc::templ::dump_format_t format)
{
    xc::templ::preproc_t pre(scan);
    dump_raw(out, pre, format);
    //dump_raw(out, scan, format);
}



