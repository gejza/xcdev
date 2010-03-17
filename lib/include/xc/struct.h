/*
   File name:  struct.h
   Date:       2010/03/17 01:56
   Subversion: $Id$
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_STRUCT_H_
#define _XC_STRUCT_H_
#pragma once

#include "value.h"

namespace xc {

class struct_t : public value_t {
public:
    struct_t() {}
    virtual ~struct_t() {}
};

} // namespace xc

#endif // _XC_STRUCT_H_
/* end of struct.h */

