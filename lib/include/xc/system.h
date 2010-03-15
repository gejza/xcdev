
/*
* Copyright
*/

#pragma once
#ifndef _SYSTEM_FUNCTIONS_H_
#define _SYSTEM_FUNCTIONS_H_

#include "tree.h"
#include "system.h"

#ifdef __cplusplus
extern "C" {
#endif

// todo vyresit typ
typedef long threadid_t;
#ifdef _WIN32
typedef long mutex_t;
#else
typedef long mutex_t;
#endif

threadid_t system_get_thread_id();

mutex_t system_mutex_create();
void system_mutex_delete(mutex_t mutex);
void system_lock(mutex_t mutex);
void system_unlock(mutex_t mutex);

#ifdef __cplusplus 
} // extern "C"
#endif

#endif // _SYSTEM_FUNCTIONS_H_
