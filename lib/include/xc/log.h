/*
   File name:  log.h
   Date:       2010/03/17 03:10
   Subversion: $Id$
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

/**
 * @example log.cc
 */

#ifndef _XC_LOG_H_
#define _XC_LOG_H_
#pragma once

#include <stdarg.h>
#include <xc/debug.h>

namespace xc {
namespace log {

    void add_file(const char* path, const char* mask);
    void add_stderr(const char* mask);

    void logrotate();

} // namespace log

/** 
 * @short 
 * @param level 
 * @param format 
 */
void log_info(const debug::loc_t& loc, int level, const char* format, ...);// __attribute__((format(printf, 1, 2)));

/** 
 * @short 
 * @param level 
 * @param format 
 * @param arg 
 */
void logv_info(const debug::loc_t& loc, int level, const char* format, va_list arg);// __attribute__((format(printf, 1, 2)));

/** 
 * @short 
 * @param level 
 * @param format 
 */
void log_warn(const debug::loc_t& loc, int level, const char* format, ...);// __attribute__((format(printf, 1, 2)));

/** 
 * @short 
 * @param level 
 * @param format 
 * @param arg 
 */
void logv_warn(const debug::loc_t& loc, int level, const char* format, va_list arg);// __attribute__((format(printf, 1, 2)));

/** 
 * @short 
 * @param level 
 * @param format 
 */
void log_error(const debug::loc_t& loc, int level, const char* format, ...);// __attribute__((format(printf, 1, 2)));

/** 
 * @short 
 * @param level 
 * @param format 
 * @param arg 
 */
void logv_error(const debug::loc_t& loc, int level, const char* format, va_list arg);// __attribute__((format(printf, 1, 2)));

/** 
 * @short 
 * @param format 
 */
void log_fatal(const debug::loc_t& loc, const char* format, ...);// __attribute__((format(printf, 1, 2)));

/** 
 * @short 
 * @param level 
 * @param format 
 * @param arg 
 */
void logv_fatal(const debug::loc_t& loc, const char* format, va_list arg);// __attribute__((format(printf, 1, 2)));

#ifdef LOGBACKTRACE
#define DEBUG_INFO XC_LOC_TRACE,
#else
#define DEBUG_INFO XC_LOC_FILE,
#endif

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

