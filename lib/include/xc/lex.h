/*
   File name:  lex.h
   Date:       2010/03/26 12:27
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_LEX_H_
#define _XC_LEX_H_
#pragma once

#include <xc/file.h>
#ifndef XCLEX
#define XCLEX(fnc) yy##fnc
#endif

namespace xc {

/**
 * @short lex_t
 */
template<typename _scan_t>
class lex_t
{
public:

    /**
     * Default constructor
     */
    lex_t() {
        XCLEX(lex_init)(&_lex);
    }

    /**
     * Constructor
     * @param file Handle to file
     */
    lex_t(FILE* file) {
        XCLEX(lex_init)(&_lex);
        init(file);
    }

    /**
     * Constructor
     * @param file Handle to file
     */
    lex_t(file_t& file) {
        XCLEX(lex_init)(&_lex);
        init(file.handle());
    }

    /**
     * Destructor
     */
    ~lex_t() {
        XCLEX(lex_destroy)(_lex);
    }


    /** 
     * @short 
     * @param file 
     */
    void init(FILE* file) {
        XC_TRACE("yyrestart flex width FILE=%p", file);
        if (file)
            XCLEX(restart)(file, _lex);
        else
            LOG_WARN( 1, "Restarting flex width null file handle");
    }

    /** 
     * @short 
     * @return 
     */
    int lex() {
        return XCLEX(lex)(_lex);
    }

    /** 
     * @short 
     * @return 
     */
    const char* text() const {
        return XCLEX(get_text)(_lex);
    }

    /** 
     * @short 
     * @return 
     */
    size_t length() const {
        return XCLEX(get_leng)(_lex);
    }

    /** 
     * @short 
     * @return 
     */
    xc::string str() const {
        return xc::string(this->text(), this->length());
    }

    /** 
     * @short 
     * @return 
     */
    int lineno() const {
        return XCLEX(get_lineno)(_lex);
    }

    /** 
     * @short 
     * @param enable 
     */
    void debug(bool enable) {
        XCLEX(set_debug)(enable ? 1:0, _lex);
    }

private:
    lex_t(const lex_t&);
    lex_t& operator=(const lex_t&);

    _scan_t _lex;
};

template<typename _scan_t>
class lex2_t : public lex_t<_scan_t>
{
public:
    /** 
     * @short Constructor
     * @param fn 
     */
    lex2_t(const char* fn)
        : _file(fn, "rt")
    {
        this->init(_file.handle());
        //yyset_lineno(1, _scan);
    }
    
    /** 
     * @short Get Filename
     */
    const char* filename() const {
        return _file.name();
    }

private:
    xc::file_t _file;
};

typedef lex_t<void*> flex_t;
typedef lex2_t<void*> flex2_t;

} // namespace xc

#endif // _XC_LEX_H_
/* end of lex.h */

