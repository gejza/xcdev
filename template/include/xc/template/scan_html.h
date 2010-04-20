/*
   File name:  scan_html.h
   Date:       2010/03/26 12:45
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_TEMPLATE_SCAN_HTML_H_
#define _XC_TEMPLATE_SCAN_HTML_H_
#pragma once

#include <xc/string.h>
#include <xc/lex.h>

#include "scanner.h"

namespace xc {
namespace templ {

/*class Parser;
class HtmlFile {
public:
    struct Value {
        const char* str;
        size_t length;
    };

    HtmlFile(const char* filename);
    ~HtmlFile();
    int lex(Value& val);
    int getLine() const;
protected:
    int lex();
private:
    FILE* m_in;
    void* m_scanner;
    unsigned int m_line;
    int m_echo;
};*/


/**
 * @short html_file_t
 */
class html_file_t : public scanner_t
{
public:
    /**
     * Default constructor
     */
    html_file_t(const char* filename);

    /**
     * Destructor
     */
    ~html_file_t();

    /** 
     * @short 
     * @return 
     */
    virtual const term_t read();
    
    virtual const char* file_name() const {
        return _file.name();
    }
private:
    html_file_t(const html_file_t&);
    html_file_t& operator=(const html_file_t&);

    file_t _file;
    flex_t _lex;
};


} // namespace templ
} // namespace xc

#endif // _XC_TEMPLATE_SCAN_HTML_H_
/* end of scan_html.h */

