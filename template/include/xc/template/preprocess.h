/*
   File name:  preprocess.h
   Date:       2010/03/26 21:07
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_TEMPLATE_PREPROCESS_H_
#define _XC_TEMPLATE_PREPROCESS_H_
#pragma once

#include <stack>

#include <xc/string.h>
#include "scanner.h"
#include "fs.h"

namespace xc {
namespace templ {

/**
 * @short preproc_t
 */
class preproc_t : public scanner_t
{
public:

    /**
     * Default constructor
     */
    preproc_t(files_t& files)
        : _files(files), _curr(0x0), _last(TI_END) {}

    /**
     * Destructor
     */
    virtual ~preproc_t() {}

    void set(const char* name);

    virtual const scanner_t::term_t read();

    virtual const char* file_name() const {
        return _curr ? _curr->file_name() : "";
    }
private:
    preproc_t(const preproc_t&);
    preproc_t& operator=(const preproc_t&);

    files_t& _files;
    scanner_t* _curr;
    std::stack<scanner_t*> _stack;
    scanner_t::term_t _last;
    xc::string _buffer;
};

} // namespace templ
} // namespace xc

#endif // _XC_TEMPLATE_PREPROCESS_H_
/* end of preprocess.h */

