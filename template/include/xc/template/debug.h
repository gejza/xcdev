/*
   File name:  debug.h
   Date:       2010/03/26 21:16
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_TEMPLATE_DEBUG_H_
#define _XC_TEMPLATE_DEBUG_H_
#pragma once

#include <stdio.h>

namespace xc {
namespace templ {

    class scanner_t;

    enum dump_format_t {
        DF_COLOR = 1,
        DF_LIST,
    };

    void dump(FILE* out, scanner_t& scan, dump_format_t format = DF_LIST);

} // namespace templ


} // namespace xc

#endif // _XC_TEMPLATE_DEBUG_H_
/* end of debug.h */

