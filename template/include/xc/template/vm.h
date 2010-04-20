/*
   File name:  vm.h
   Date:       2010/03/26 19:20
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_TEMPLATE_VM_H_
#define _XC_TEMPLATE_VM_H_
#pragma once

#include <stdint.h>
#include "fragment.h"

namespace xc {
namespace templ {

class page_t;
class env_t;
class frag_t;

/**
 * @short vm_t
 */
class vm_t
{
public:

    /**
     * Default constructor
     */
    vm_t();

    /**
     * Destructor
     */
    virtual ~vm_t();

    void process(const page_t& page, const frag_t& frag, uint32_t ci = 0);
    void process(const page_t& page, const frag_list_t& frag, uint32_t ci) {
        for (frag_list_t::const_iterator i = frag.begin();i != frag.end(); i++) {
            process(page, *i, ci);
        }
    }

    void flush();
protected:

    virtual void vm_output(const char* ptr, size_t data) = 0;
    virtual void vm_debug(const char* ptr, size_t data) = 0;
    void vm_debug(const xc::string& str) {
        this->vm_debug(str.data(), str.size());
    }
    virtual void vm_page(const xc::string& name) = 0;
    virtual void vm_dict(const xc::string& name) = 0;
    virtual void vm_value(const xc::string& name) = 0;
private:
    vm_t(const vm_t&);
    vm_t& operator=(const vm_t&);
};


} // namespace templ


} // namespace xc

#endif // _XC_VM_H_
/* end of vm.h */

