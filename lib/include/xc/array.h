/*
   File name:  array.h
   Date:       2010/03/17 01:57
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_ARRAY_H_
#define _XC_ARRAY_H_
#pragma once

#include "value.h"

namespace xc {

class array_t : public value_t {
public:
    array_t() {}
    virtual ~array_t() {}
};

} // namespace xc

#endif // _XC_ARRAY_H_
/* end of array.h */

