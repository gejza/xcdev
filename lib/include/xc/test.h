/*
   File name:  test.h
   Date:       2010/12/30 22:40
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan.dunghubel@gmail.com>

   Copyright (C) 2010 Milan Dunghubel <milan.dunghubel@gmail.com>
*/

#ifndef _XC_TEST_H_
#define _XC_TEST_H_
#pragma once

#include <stdarg.h>

namespace xc {
namespace test {

void printf(const char *fmt, ...);
void vprintf(const char *fmt, va_list ap);

} // namespace test
} // namespace xc

#endif // _XC_TEST_H_
/* end of test.h */

