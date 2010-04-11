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

#include <xc/string.h>
#include "scanner.h"

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
    preproc_t(scanner_t& scanner)
        : _scanner(scanner), _last(TI_END) {}

    /**
     * Destructor
     */
    virtual ~preproc_t() {}

    virtual const scanner_t::term_t read();

private:
    preproc_t(const preproc_t&);
    preproc_t& operator=(const preproc_t&);

    scanner_t& _scanner;
    scanner_t::term_t _last;
    xc::string _buffer;
};

} // namespace templ
} // namespace xc

#endif // _XC_TEMPLATE_PREPROCESS_H_
/* end of preprocess.h */

