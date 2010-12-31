/*
   File name:  output.h
   Date:       2010/12/29 21:42
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan.dunghubel@gmail.com>

   Copyright (C) 2010 Milan Dunghubel <milan.dunghubel@gmail.com>
*/

#ifndef _XC_RD_OUTPUT_H_
#define _XC_RD_OUTPUT_H_
#pragma once

#include <xc/data.h>
#include <xc/refobj.h>

#include "types.h"

namespace xc {
namespace rd {

/**
 * Output class
 */
class Output_t : public xc::refobj_t
{
public:
    Output_t() {}
    virtual ~Output_t() {}

    virtual void insert(const xc::rd::ns_t ns, const xc::data_t& key,
			const xc::data_t& value) = 0;
};

} // namespace rd
} // namespace xc

#endif // _XC_RD_OUTPUT_H_
/* end of output.h */

