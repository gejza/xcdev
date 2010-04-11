/*
   File name:  value.h
   Date:       2010/03/17 01:54
   Subversion: $Id$
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_VALUE_H_
#define _XC_VALUE_H_
#pragma once

namespace xc {

class value_t {
public:
    value_t() {}
    virtual ~value_t() {}

    virtual unsigned int type() const = 0;
    virtual const char* type_name() const = 0;
};

class struct_t : public value_t {
};

} // namespace xc

#endif // _XC_VALUE_H_
/* end of value.h */

