/*
   File name:  page.h
   Date:       2010/03/26 21:15
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_TEMPLATE_PAGE_H_
#define _XC_TEMPLATE_PAGE_H_
#pragma once

#include <xc/refobj.h>

namespace xc {
namespace templ {

/**
 * @short page_t
 */
class page_t : xc::refobj_t
{
public:

    /**
     * Default constructor
     */
    page_t() {}

    /**
     * Destructor
     */
    virtual ~page_t() {}

    unsigned char* code() const {
        return _code;
    }

    const char* data(unsigned int ptr) const {
        return _data + ptr;
    }
private:
    page_t(const page_t&);
    page_t& operator=(const page_t&);
protected:
    unsigned char* _code;
    const char* _data;
};


} // namespace templ
} // namespace xc

#endif // _XC_TEMPLATE_PAGE_H_
/* end of page.h */

