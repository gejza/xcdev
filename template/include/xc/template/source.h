/*
   File name:  source.h
   Date:       2010/03/26 21:16
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_TEMPLATE_SOURCE_H_
#define _XC_TEMPLATE_SOURCE_H_
#pragma once

namespace xc {
namespace templ {

// forward
class page_t;

/**
 * @short source_t
 */
class source_t
{
public:

    /**
     * Default constructor
     */
    source_t();

    /**
     * Destructor
     */
    ~source_t();

    virtual page_t* get_page(const char* name) = 0;

private:
    source_t(const source_t&);
    source_t& operator=(const source_t&);
};


} // namespace templ
} // namespace xc

#endif // _XC_TEMPLATE_SOURCE_H_
/* end of source.h */

