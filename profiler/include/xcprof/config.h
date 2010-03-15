
/*
* Copyright
*/

#pragma once
#ifndef _XCPROF_CONFIG_H_
#define _XCPROF_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32) && defined(XC_SHARED)
#ifdef XC_LIB
#define XCP_API __declspec(dllexport)
#else
#define XCP_API __declspec(dllimport)
#endif
#else
#define XCP_API
#endif

#ifdef __cplusplus 
} // extern "C"
#endif

#endif // _XCPROF_CONFIG_H_
