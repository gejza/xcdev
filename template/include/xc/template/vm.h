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

namespace xc {
namespace templ {

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
    ~vm_t();

private:
    vm_t(const vm_t&);
    vm_t& operator=(const vm_t&);
};


} // namespace templ


} // namespace xc

#endif // _XC_VM_H_
/* end of vm.h */

