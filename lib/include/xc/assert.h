/*
   File name:  assert.h
   Date:       2010/12/12 23:37
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_ASSERT_H_
#define _XC_ASSERT_H_
#pragma once

#ifdef	assert
//#error "std assert defined."
#else

namespace xc {

#define ENABLE_ASSERT 1

#ifdef ENABLE_ASSERT

    /* Version 2.4 and later of GCC define a magical variable `__PRETTY_FUNCTION__'
   which contains the name of the function currently being defined.
   This is broken in G++ before version 2.6.
   C9x has a similar variable called __func__, but prefer the GCC one since
   it demangles C++ function names.  */
# if defined __cplusplus ? __GNUC_PREREQ (2, 6) : __GNUC_PREREQ (2, 4)
#   define __ASSERT_FUNCTION	__PRETTY_FUNCTION__
# else
#  if defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L
#   define __ASSERT_FUNCTION	__func__
#  else
#   define __ASSERT_FUNCTION	((const char *) 0)
#  endif
# endif

    /* This prints an "Assertion failed" message and aborts.  */
    void __assert_fail (const char *__assertion, const char *__file,
                   unsigned int __line, const char *__function)
         __THROW __attribute__ ((__noreturn__));

    /* The following is not at all used here but needed for standard
       compliance.  */
    void __assert (const char *__assertion, const char *__file, int __line)
         __THROW __attribute__ ((__noreturn__));

# define assert(expr)							\
      ((expr)								\
       ? (void)(0)						\
       : __assert_fail (__STRING(expr), __FILE__, __LINE__, __ASSERT_FUNCTION))

#else

# define assert(expr)		((void) (0))

#endif

} // namespace xc

#endif

#endif // _XC_ASSERT_H_
/* end of assert.h */
