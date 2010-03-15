
/*
* Copyright
*/

#pragma once
#ifndef _PROFILE_TIMES_H_
#define _PROFILE_TIMES_H_

#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	TIME_UNIT_MICRO,
	TIME_UNIT_MILI,
	TIME_UNIT_SEC,
	TIME_UNIT_MIN,
	TIME_UNIT_HOUR,
} TIME_UNIT;

typedef clock_t time_watch_type_t;

/** Structure for time watch */
typedef struct _time_watch_t
{
	time_watch_type_t start; ///< start operation
	time_watch_type_t finish; ///< finish operation
	size_t nums; ///< num operations
} time_watch_t;

//todo doc
XCP_API void watch_start(time_watch_t* time);
XCP_API void watch_finish(time_watch_t* time, size_t numop);
XCP_API void watch_report(const time_watch_t* time, FILE* out);
XCP_API double watch_one(const time_watch_t* time);

#ifdef __cplusplus 
} // extern "C"
#endif

#endif // _PROFILE_TIMES_H_
