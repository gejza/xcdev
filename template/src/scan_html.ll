/* -*- C++ -*-
   File name:  scan_html.ll
   Date:       2010/03/26 12:39
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
%top{
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
    
#define XC_DEBUG_IDENT xc::debug::USER
#define XCLEX(fnc) xct##fnc

}
%{
#include <stdio.h>
#include <xc/string.h>
#include <xc/log.h>
#include <xc/file.h>
#include <xc/error.h>
#include <xc/lex.h>
#include <xc/text.h>
#include "xc/template/bytecode.h"
#include "xc/template/scan_html.h"

///////////////////////////////////////
// Helper functions
namespace {

    int m_echo = xc::templ::TI_TEXT;
}

%}

%option 8bit reentrant default
%option batch
%option nounput
%option noyywrap
%option yylineno
%option verbose
%option warn
%option debug
%option prefix="xct"
%option outfile="lex.yy.c"

%x COMMENT
%x PRE

DIGIT   [0-9]
NAME    [A-Za-z_]+[A-Za-z0-9_-]*
WS      [\t ]+
STR1    \"[^\"]*\"
STR2    '[^']*'

%%


^%include{WS}{STR1}.*$  return xc::templ::TI_INCLUDE;
^%include{WS}{STR2}.*$  return xc::templ::TI_INCLUDE;
^%{NAME}                return xc::templ::TI_SECTION;

"<!"[^>]+">"            return xc::templ::TI_RAW;
"<<"{NAME}">>"          return xc::templ::TI_PAGE;
"[["{NAME}"]]"          return xc::templ::TI_PAGE_LIST;

"#{"[^\}]*"}"           return xc::templ::TI_DICT;
"${"[^\}]*"}"           return xc::templ::TI_VALUE;
"#${"[^\}]*"}"          return xc::templ::TI_DICT_VALUE;
"$#{"[^\}]*"}"          return xc::templ::TI_DICT_VALUE;

"<style>"               m_echo = xc::templ::TI_RAW; return xc::templ::TI_RAW;
"</style>"              m_echo = xc::templ::TI_TEXT; return xc::templ::TI_RAW;
"<"{NAME}">"            m_echo = xc::templ::TI_TEXT; return xc::templ::TI_RAW;
"<"{NAME}               m_echo = xc::templ::TI_RAW; return xc::templ::TI_RAW;
">"                     m_echo = xc::templ::TI_TEXT; return xc::templ::TI_RAW;

"</"[^>]*">"            m_echo = xc::templ::TI_TEXT; return xc::templ::TI_RAW;

"*"{NAME}"*"            return xc::templ::TI_FRAG_BEGIN;
"#"{NAME}"#"            return xc::templ::TI_FRAG_END;

{WS}+                   return m_echo;
{NAME}                  return m_echo;

"{*"                    BEGIN(COMMENT);
<COMMENT>"*}"           BEGIN(INITIAL);
<COMMENT>[^\*]*         /* eat anything that's not a '*' */
<COMMENT>\*             /* eat up '*'s */
<COMMENT>\n             /* eat new line */

"{{{"                   BEGIN(PRE);
<PRE>"}}}"              BEGIN(INITIAL);
<PRE>[^}]*              return xc::templ::TI_RAW;
<PRE>\}                 return xc::templ::TI_RAW;
<PRE>\n                 return xc::templ::TI_RAW;

.                       return m_echo;
\n                      return m_echo;


%%

xc::templ::html_file_t::html_file_t(const char* filename)
    : _file(filename), _lex(_file.handle())
{
    LOG_INFO(1, "Open template file `%s'", filename);
    //_lex.debug(true);
}

xc::templ::html_file_t::~html_file_t()
{
}

const xc::templ::html_file_t::term_t xc::templ::html_file_t::read()
{
    int type = _lex.lex();
    switch (type) {
    case TI_TEXT:
        type = TI_RAW;
        break;
    default:
        break;
    };

    term_t term(type);
    if (term) {
        term.text = _lex.text();
        term.length = _lex.length();
        term.line = _lex.lineno();
    }
    XC_DBG("lex: read token %s '%s', echo=%s", term.name(), xc::human(term.text, 50).c_str(), ti_name(m_echo)); 
    return term;
}

/*
HtmlFile::HtmlFile(const char* filename)
    : m_in(NULL), m_scanner(NULL), m_line(1), m_echo(TI_DICT)
{
    yylex_init(&m_scanner);
    if (filename)
    {
        m_in = fopen(filename, "r");
        if (!m_in)
            throw std::runtime_error(std::string("file ") + filename + " not found");
        yyrestart(m_in, m_scanner);
    }
    //yyset_lineno(1, m_scanner);
    //yyset_debug(1, m_scanner);
}

HtmlFile::~HtmlFile()
{
    if (m_scanner)
        yylex_destroy(m_scanner);
    if (m_in)
        ::fclose(m_in);
}

int HtmlFile::getLine() const
{
    return m_line;
}

int HtmlFile::lex(HtmlFile::Value& val)
{
    int t = yylex(m_scanner);
    val.str = yyget_text(m_scanner);
    val.length = yyget_leng(m_scanner);
    return t;
}
*/

/* 
 * vim: ft=cpp
 */

