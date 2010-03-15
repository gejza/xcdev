
/*
* Copyright
*/

#pragma once
#ifndef _UDUMP_UTIL_H_
#define _UDUMP_UTIL_H_

#include "config.h"
#include "xc/tree.h"
#include "xc/system.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GV_SYMBOL(type,name) _glob_##type##_##name

#if PROFILER_TS

/** Thread safe structure for records */
typedef struct _threadsafe_t
{
	tree_t child;
	pool_t pool;
	int ref;
	mutex_t mutex;
} threadsafe_t;

void threadsafe_inc(threadsafe_t* ts);
void threadsafe_dec(threadsafe_t* ts);
void* threadsafe_get_by_id(threadsafe_t* ts, threadid_t id, size_t size);

#define GV_SYMBOL_TS(type,name) _glob_##type##_##name##_ts
#define GV_SYMBOL_MUTEX(type,name) GV_SYMBOL_TS(type,name)##_mutex

#define GV_DECL(type,name) static type GV_SYMBOL(type,name) = {0};\
	static mutex_t GV_SYMBOL_MUTEX(type,name) = system_mutex_create();
#define GV_DECL_TS(type,name) static threadsafe_t GV_SYMBOL_TS(type,name) = {0}
#define GV_INC_TS(type,name) threadsafe_inc(& GV_SYMBOL_TS(type,name))
#define GV_DEC_TS(type,name) threadsafe_dec(& GV_SYMBOL_TS(type,name))
#define GV_LOCK(type,name) (system_lock(GV_SYMBOL_MUTEX(type,name)),& GV_SYMBOL(type,name))
#define GV_UNLOCK(type,name) system_unlock(GV_SYMBOL_MUTEX(type,name))
#define GV_TS_ID(type,name, id) ((type*) threadsafe_get_by_id \
	(& GV_SYMBOL_TS(type,name), id, sizeof(type)))
#define GV_TS(type,name) GV_TS_ID(type, name, system_get_thread_id())
#else
#define GV_DECL(type,name) static type GV_SYMBOL(type,name) = {0}
#define GV_DECL_TS(type,name) GV_DECL(type,name)
#define GV_INC_TS(type,name)
#define GV_DEC_TS(type,name)
#define GV_LOCK(type,name) (& GV_SYMBOL(type,name))
#define GV_UNLOCK(type,name) 
#define GV_TS(type,name) GV_LOCK(type,name)
#endif

#ifdef __cplusplus 
} // extern "C"
#endif

#endif // _UDUMP_UTIL_H_
