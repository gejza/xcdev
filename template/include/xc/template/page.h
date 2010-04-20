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
#include <xc/string.h>
#include <stdint.h>

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
    page_t(const xc::string& name) : _name(name) {}

    /**
     * Destructor
     */
    virtual ~page_t() {}

    virtual const unsigned char* code(uint32_t addr) const = 0;

    virtual const char* data(unsigned int ptr) const = 0;

    const char* name() const {
        return _name.c_str();
    }
private:
    page_t(const page_t&);
    page_t& operator=(const page_t&);
protected:
    xc::string _name;
};


} // namespace templ
} // namespace xc

#endif // _XC_TEMPLATE_PAGE_H_
/* end of page.h */

