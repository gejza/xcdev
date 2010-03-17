
/*
* Copyright
*/

#pragma once
#ifndef _XC_DEBUG_HPP_
#define _XC_DEBUG_HPP_

#include "config.h"

namespace xc {
namespace debug {
	XC_API void EnableDebug(bool enable);

	XC_API void Print(int type, const char* format, ...);
	XC_API void PrintV(int type, const char* format, va_list args);
	// print debug
	// addref for object
	// decref for object
	// constructors & destructors

	XC_API void Assert(bool exp, const char* file, int line, const char* format, ...);
} //namespace debug
} // namespace xc

#ifdef _WIN32
#define XC_DBG(format, ...) xc::debug::Print(format, __VA_ARGS__)
#else
#define XC_DBG(format...) xc::debug::Print(format)
#endif
#define XC_ASSERT(_Expression) (void)( (!!(_Expression)) || (xc::debug::Assert(false, __FILE__, __LINE__, #_Expression), 0))

#define TRACE_WARN(level, format...) ::Itissien::_i_trace_warn(DEBUG_INFO, ::Itissien::Trace(0), level, format)

#endif // _XC_DEBUG_HPP_
