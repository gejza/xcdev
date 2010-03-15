
/*
* Copyright
*/

#pragma once
#ifndef _PROFILER_H_
#define _PROFILER_H_

#include "config.h"
#include <xc/symboltable.h>
#include "udump.h"
#include "times.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _xc_profiler_t
{
	udump_t dump;
} xc_profiler_t;

enum {
	PT_INT,
	PT_STRING,
};

XCP_API int xcprof_init(xc_profiler_t* profiler);
XCP_API void xcprof_destroy(xc_profiler_t* profiler);
XCP_API void xcprof_begin(xc_profiler_t* profiler, const char* entryname);
XCP_API void xcprof_end(xc_profiler_t* profiler);
XCP_API void xcprof_child_init();
XCP_API void xcprof_child_destroy();

XCP_API void xcprof_value_int(const char* symbol, int value SRC_DECL);
XCP_API void xcprof_value_double(const char* symbol, double value SRC_DECL);
XCP_API void xcprof_value_userdata(const char* symbol, const void* ptr, size_t size SRC_DECL);
XCP_API void xcprof_value_string(const char* symbol, const char* str SRC_DECL);
XCP_API void xcprof_value_symbol(const char* symbol, const char* str SRC_DECL);
XCP_API void xcprof_value_watch(const char* symbol, const time_watch_t* watch SRC_DECL);
XCP_API void xcprof_value_checkpoint(const char* symbol, 
							   const char* srcfile, int srcline, const char* function);

XCP_API void xcprof_dumpsysinfo(xc_profiler_t* profiler SRC_DECL);
XCP_API void xcprof_value_traceback(const char* symbol SRC_DECL);

#define PROF_INT(symbol, value) xcprof_value_int(symbol, value SRC_INFO)
#define PROF_FLOAT(symbol, value) xcprof_value_double(symbol, value SRC_INFO)
#define PROF_USERDATA(symbol, ptr, size) \
	xcprof_value_userdata(symbol, ptr, size SRC_INFO)
#define PROF_STRING(symbol, value) xcprof_value_string(symbol, value SRC_INFO)
#define PROF_SYMBOL(symbol, value) xcprof_value_symbol(symbol, value SRC_INFO)
#define PROF_CHECKPOINT(symbol) xcprof_value_checkpoint(symbol, __FILE__, __LINE__, __FUNCTION__) 
#define PROF_TRACEBACK(symbol) xcprof_value_traceback(symbol SRC_INFO) 

// empty macro for simple find default symbols
#define _SYM(s) (s)

#ifdef __cplusplus 
} // extern "C"
#endif

#endif // _PROFILER_H_
