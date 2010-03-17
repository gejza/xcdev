/*
   File name:  log.h
   Date:       2010/03/17 03:10
   Subversion: $Id$
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_LOG_H_
#define _XC_LOG_H_
#pragma once

#include <stdarg.h>

namespace xc {

void log_info(int level, const char* format, ...);// __attribute__((format(printf, 1, 2)));
void logv_info(int level, const char* format, va_list arg);// __attribute__((format(printf, 1, 2)));

void log_warn(int level, const char* format, ...);// __attribute__((format(printf, 1, 2)));
void logv_warn(int level, const char* format, va_list arg);// __attribute__((format(printf, 1, 2)));

void log_error(int level, const char* format, ...);// __attribute__((format(printf, 1, 2)));
void logv_error(int level, const char* format, va_list arg);// __attribute__((format(printf, 1, 2)));

void log_fatal(const char* format, ...);// __attribute__((format(printf, 1, 2)));
void logv_fatal(const char* format, va_list arg);// __attribute__((format(printf, 1, 2)));

#define DEBUG_INFO

#ifndef LOG_INFO
#define LOG_INFO(level, format...) ::xc::log_info(DEBUG_INFO level, format)
#endif

#ifndef LOG_WARN
#define LOG_WARN(level, format...) ::xc::log_warn(DEBUG_INFO level, format)
#endif

#ifndef LOG_ERROR
#define LOG_ERROR(level, format...) ::xc::log_error(DEBUG_INFO level, format)
#endif

#ifndef LOG_FATAL
#define LOG_FATAL(format...) ::xc::log_fatal(DEBUG_INFO format)
#endif


} // namespace xc

#endif // _XC_LOG_H_
/* end of log.h */

