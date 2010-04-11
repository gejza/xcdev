/*
   File name:  error.h
   Date:       2010/03/26 21:53
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_TEMPLATE_ERROR_H_
#define _XC_TEMPLATE_ERROR_H_
#pragma once

#include <xc/error.h>

namespace xc {
namespace templ {

/**
 * @short error_t
 */
class error_t : public ::xc::error_t
{
public:

    /**
     * Default constructor
     */
    error_t(const ::xc::string& msg)
        : xc::error_t(msg) {}

};


} // namespace templ
} // namespace xc

#endif // _XC_TEMPLATE_ERROR_H_
/* end of error.h */

