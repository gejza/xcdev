/*
   -*- C++ -*-
   File name:  parser.h
   Date:       2010/02/05 18:07
   Subversion: $Id$
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
%top{
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define XC_DEBUG_IDENT xc::debug::CONFIG
#define XCLEX(fnc) xccini##fnc
#define YY_FATAL_ERROR(msg) ERROR(xc::error_t, "Lex fatal error: %s", msg)
}
%{
#include <xc/string.h>
#include <xc/log.h>
#include <xc/file.h>
#include <xc/error.h>
#include <xc/config.h>
#include <xc/lex.h>

#include <memory>

enum {
    T_NL = 1 << 10,
    T_EMPTY,
    T_SECTION,
    T_NAME,
    T_VALUE,
};

%}

%option 8bit reentrant default
%option extra-type="int"
%option batch
%option nounput
%option noyywrap
%option verbose
%option warn
%option debug
%option yylineno
%option prefix="xccini"
%option outfile="lex.yy.c"

%x ERR

DIGIT   [0-9]
NAME    [A-Za-z._-]+[A-Za-z0-9._-]*
WS      [\t ]+
TS      ([^'\\]|\\')
STR1    \"([^\"\n]|\\\")*\"
STR2    '([^'\n]|\\')*'
CMT     [#;].*

%%

^{WS}?\r?\n                         return T_EMPTY;

#.*$                                /* skip */
;.*$                                /* skip */
{WS}                                /* skip */
\r?\n                               return T_NL;

\[[A-Za-z0-9:_.]+\]                 return T_SECTION;
{NAME}                              return T_NAME;

=({STR1}|{STR2}|[^;#\n])*           return T_VALUE;
.                                   return yytext[0];



%%
/*={WS}?{STR1}{WS}?                   return T_VALUE;
={WS}?{STR2}{WS}?                   return T_VALUE;
=([^;#]|\\[;#])*                    return T_VALUE;
*/
//^%include{WS}{STR1}.*$  return 1;
//^%include{WS}{STR2}.*$  return 1;

namespace {

using namespace xc;
using namespace xc::config;

//typedef xc::lex2_t<yyscan_t>  flex_t;

xc::string parse_header(flex2_t& l)
{
    xc::string name = xc::string(l.text()+1, l.length()-2);
    XC_DBG("Sekce %s", name.c_str());
    if (l.lex() != T_NL) {
        ERROR(xc::config::error_t, "%s:%d: Unknown `%s'.", l.filename(), l.lineno(), l.text());
    }
    return name;
}

void parse_name(flex2_t& l, dyn_section_t* sec)
{
    xc::string name = l.str();
    XC_DBG("Name %s", name.c_str());
    switch (l.lex()) {
    case T_VALUE:
        break;
    case T_NL:
        ERROR(xc::config::error_t, "%s:%d: Missing `='.", l.filename(), l.lineno());
    default:
        ERROR(xc::config::error_t, "%s:%d: Unknown `%s'. Expect `='.", l.filename(), l.lineno(), l.text());
    }
    sec->set(name.c_str(),l.text()+1);
    XC_DBG("Value `%s'", l.text()+1);
}

}

namespace xc {
namespace config {

void parse_ini_file(const char* fn)
{
    XC_TRACE("Init flex for scan file %s", fn);
    flex2_t l(fn);
    xc::string sname; 
    std::auto_ptr<dyn_section_t> sec;
    int el = 0; // empty line
    while (int t = l.lex()) {
        //LOG_INFO(1, "Found token %d - `%s' at %d", t, l.text(), l.lineno());
        switch (t) {
        case T_SECTION:
            if (sec.get()) {
                xc::config::add_section(sname.c_str(), sec.release());
                sname.clear();
            }
            sname = parse_header(l);
            sec.reset(new xc::config::dyn_section_t());
            el = 0;
            break;
        case T_EMPTY:
            if (!el && sec.get())
                el = l.lineno() - 1;
            continue;
        case T_NL:
            continue;
        case T_NAME:
            if (!sec.get())
                goto err;
            if (el)
                LOG_WARN(3, "%s:%d: Empty line found at line %d", l.filename(), el);
            el = 0;
            parse_name(l, sec.get());
            break;
        default:
            goto err;
        };
    }

    if (sec.get()) {
        xc::config::add_section(sname.c_str(), sec.release());
        sname.clear();
    }
    //yyget_text(m_scanner);
    //yyget_leng(m_scanner);
    //std::auto_ptr<dyn_section_t> sec(new dyn_section_t());
    //sec->set("Message", "Ahoj vole");
    //add_section("test", sec.release());
    //    LOG_INFO(1, "Found section: %s", yytext);

    return;
err:
    ERROR(xc::config::error_t, "%s:%d: Unknown `%s'. Expect header section.", l.filename(), l.lineno(), l.text());
}

} //namespace config
} // namespace xc

/* 
 * vim: ft=cpp
 */

