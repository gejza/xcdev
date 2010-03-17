/*
   File name:  log.cc
   Date:       2010/03/17 03:33
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <time.h>

#include "xc/log.h"

namespace {
    void print_time(FILE* out)
    {
        char outstr[200];
        time_t t;
        struct tm *tmp;

        t = ::time(NULL);
        tmp = ::localtime(&t);
        if (tmp == NULL)
            return;

        if (::strftime(outstr, sizeof(outstr), "[%T] ", tmp) == 0)
            return;

        ::fputs(outstr, out);
    }

    void print_procinfo(FILE* out)
    {
        ::fprintf(out, "[%d] ", ::getpid());
    }

    void print_info(FILE* out)
    {
        print_time(out);
        print_procinfo(out);
    }
    
} // namespace

void xc::log_info(int level, const char* format, ...)
{
    va_list vl;
    va_start(vl, format);
    logv_info(level, format, vl);
    va_end(vl);
}

void xc::log_warn(int level, const char* format, ...)
{
    va_list vl;
    va_start(vl, format);
    logv_warn(level, format, vl);
    va_end(vl);
}

void xc::log_error(int level, const char* format, ...)
{
    va_list vl;
    va_start(vl, format);
    logv_error(level, format, vl);
    va_end(vl);
}

void xc::log_fatal(const char* format, ...)
{
    va_list vl;
    va_start(vl, format);
    logv_fatal(format, vl);
    va_end(vl);
}

void xc::logv_info(int level, const char* format, va_list arg)
{
    print_info(stdout);
    fprintf(stdout, "INFO%d: ", level);
    vfprintf(stdout, format, arg);
    fprintf(stdout, "\n");
}

void xc::logv_warn(int level, const char* format, va_list arg)
{
    print_info(stdout);
    bool colored = isatty(1);
    if (colored) fprintf(stdout, "\033[1;33m");
    fprintf(stdout, "WARN%d: ", level);
    vfprintf(stdout, format, arg);
    fprintf(stdout, colored ? "\033[0m":"");
    fprintf(stdout, " at %s", __FUNCTION__);
    fprintf(stdout, "\n");
}

void xc::logv_error(int level, const char* format, va_list arg)
{
    print_info(stderr);
    bool colored = isatty(2);
    if (colored) fprintf(stderr, "\033[1;31m");
    fprintf(stderr, "ERROR%d: ", level);
    vfprintf(stderr, format, arg);
    fprintf(stderr, colored ? "\033[0m":"");

    fprintf(stderr, " at %s", __PRETTY_FUNCTION__);
    fprintf(stderr, "\n");
}

void xc::logv_fatal(const char* format, va_list arg)
{
    print_info(stderr);
    bool colored = isatty(2);
    if (colored) fprintf(stderr, "\033[1;31m");
    fprintf(stderr, "FATAL: ");
    vfprintf(stderr, format, arg);
    fprintf(stderr, colored ? "\033[0m":"");
    fprintf(stderr, " at %s:%d", __FILE__, __LINE__);
    fprintf(stderr, "\nExiting...\n");
    
    exit(EXIT_FAILURE);
}

