
/*
* Copyright
*/

#pragma once
#ifndef _PROFILE_TRACEBACK_H_
#define _PROFILE_TRACEBACK_H_

#include "udump/symboltable.h"
#include "udump/udump.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _traceback_t {
	size_t num;
} traceback_t;

void traceback_dump(traceback_t* t, udump_buffer_t* buff, symboltable_t* table);


#ifdef __cplusplus 
} // extern "C"
#endif

#endif // _PROFILE_TRACEBACK_H_
