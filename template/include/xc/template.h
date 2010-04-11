/*
   File name:  template.h
   Date:       2010/03/25 18:38
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_TEMPLATE_H_
#define _XC_TEMPLATE_H_
#pragma once

#include <queue>

#include "template/error.h"
#include "template/env.h"
#include "template/vm.h"
#include "template/output.h"

namespace xc {
namespace templ {

// forward
class page_t;
class output_t;

/**
 * @short templ_t
 */
class templ_t : public vm_t
{
public:

    /**
     * Default constructor
     */
    templ_t(const env_t& env, output_t& out);

    /**
     * Destructor
     */
    ~templ_t();

    void add_page(const char* name);
private:
    templ_t(const templ_t&);
    templ_t& operator=(const templ_t&);
    
    const env_t& _env;

    std::queue<page_t*> _pages;
};



} // namespace templ
} // namespace xc

#endif // _XC_TEMPLATE_H_
/* end of template.h */

