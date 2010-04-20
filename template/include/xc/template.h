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

#include <vector>
#include <map>

#include "template/error.h"
#include "template/env.h"
#include "template/vm.h"
#include "template/output.h"
#include "template/fragment.h"

namespace xc {
namespace templ {

// forward
class page_t;

/**
 * @short templ_t
 */
class templ_t : private vm_t
{
public:

    /**
     * Default constructor
     */
    templ_t(const env_t& env);

    /**
     * Destructor
     */
    virtual ~templ_t();

    void add_page(const char* source);

    void set_page(const char* name, const char* source);

    void add_page(const char* name, const char* source);

    frag_t& root() {
        return _root;
    }
    /** 
     * @short 
     * @param out 
     */
    void generate(output_t& out);
private:
    templ_t(const templ_t&);
    templ_t& operator=(const templ_t&);

    typedef std::vector<page_t*> _page_list_t;
    typedef std::map<xc::string, _page_list_t> _page_map_t;
    
    void process(const _page_list_t& list);

    virtual void vm_output(const char* ptr, size_t data);
    virtual void vm_debug(const char* ptr, size_t data);
    virtual void vm_page(const xc::string& name);
    virtual void vm_dict(const xc::string& name);
    virtual void vm_value(const xc::string& name);

    const env_t& _env;
    _page_list_t _pagelist;
    _page_map_t _pages;
    frag_t _root;
};



} // namespace templ
} // namespace xc

#endif // _XC_TEMPLATE_H_
/* end of template.h */

