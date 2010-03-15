

/*
* Copyright
*/

#pragma once
#ifndef _MEMORY_DEBUG_H_
#define _MEMORY_DEBUG_H_

#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

XCP_API void memdbg_start();
XCP_API void memdbg_end();

#ifdef __cplusplus 
} // extern "C"
#endif

#endif // _MEMORY_DEBUG_H_

