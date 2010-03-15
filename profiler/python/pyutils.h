
/*
* Copyright
*/

#pragma once
#ifndef _PYTHON_UTIL_H_
#define _PYTHON_UTIL_H_

#include <frameobject.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CURR_FRAME (PyThreadState_Get()->frame)

static __inline const char* python_getsrcname(const PyFrameObject* frame)
{
	const PyCodeObject* code = frame->f_code;
	return PyString_AsString(code->co_filename);
}

static __inline const char* python_getfuncname(const PyFrameObject* frame)
{
	const PyCodeObject* code = frame->f_code;
	return PyString_AsString(code->co_name);
}

static __inline int python_getcodepos(const PyFrameObject* frame)
{
	return frame->f_lasti;
}

#ifdef __cplusplus 
} // extern "C"
#endif

#endif // _PYTHON_UTIL_H_
