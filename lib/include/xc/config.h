
/*
* Copyright
*/

#pragma once
#ifndef _XCLIB_CONFIG_H_
#define _XCLIB_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif
#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						
#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif
#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#endif
#include <stdlib.h>



static const int xc_api_version = 1;

#ifndef XC_THREADSAFE
#define XC_THREADSAFE 0
#endif

#ifdef _WIN32
#	define XC_DLL_EXPORT __declspec(dllexport)
#	define XC_DLL_IMPORT __declspec(dllimport)
#else
#	define XC_DLL_EXPORT
#	define XC_DLL_IMPORT
#endif

#ifdef XC_SHARED
#	ifdef XC_LIB
#		define XC_API XC_DLL_EXPORT
#	else
#		define XC_API XC_DLL_IMPORT
#	endif
#else
#	define XC_API
#endif

// 1 = FILE
#ifdef _WIN32
#define FILE_SYS 3
#define FASTCALL __fastcall
#else
#define FILE_SYS 1
#define FASTCALL
#endif

typedef unsigned char xcbyte_t;
typedef unsigned short xcuint16_t;
typedef unsigned int xcuint32_t;
typedef unsigned long long xcuint64_t;
typedef unsigned int xcbool_t;
static const unsigned int xctrue = 1;
static const unsigned int xcfalse = 0;

/**
 * Run to check if version of library is same as binary conf
 */
XC_API int xcconfig_check(int apiversion, int threadsafe);
#define XC_VERSION_CHECK xcconfig_check(xc_api_version, XC_THREADSAFE);


static __inline size_t xc_align(size_t what, size_t block)
{
	return what + (block - (what % block));
}

static __inline size_t xc_min(size_t s1, size_t s2)
{
	return s1 < s2 ? s1:s2;
}

#ifdef __cplusplus 
} // extern "C"
#endif

#endif // _XCLIB_CONFIG_H_
