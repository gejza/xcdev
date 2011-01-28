/*
   File name:  types.h
   Date:       2010/12/30 21:06
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan.dunghubel@gmail.com>

   Copyright (C) 2010 Milan Dunghubel <milan.dunghubel@gmail.com>
*/

#ifndef _XC_RD_TYPES_H_
#define _XC_RD_TYPES_H_
#pragma once

#include <xc/buffer.h>

namespace xc {
namespace rd {

	typedef uint32_t ns_t;


} // namespace rd
} // namespace xc

inline xc::buffer_t& operator << (xc::buffer_t& buff, const xc::rd::ns_t ns)
{
	buff.append(&ns, sizeof(ns));
	return buff;
}

#endif // _XC_RD_TYPES_H_
/* end of types.h */

