/*
   File name:  fs.cc
   Date:       2010/03/25 14:05
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#define XC_DEBUG_IDENT xc::debug::FS

#include <wordexp.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

#include "xc/fs.h"
#include "xc/debug.h"

xc::string xc::fs::eval_path(const char* path)
{
    wordexp_t p;
    wordexp(path, &p, 0);
    xc::string ret = p.we_wordc ? p.we_wordv[0] : path;
    wordfree(&p);
    return ret;
}

bool xc::fs::avail(const char* path, const char* name)
{
    xc::string fn(path);
    fn += '/';
    fn += name;
    return avail(fn.c_str());
}

bool xc::fs::avail(const char* filename)
{
    struct stat st;
    int ret = TEMP_FAILURE_RETRY(::stat(filename, &st));
    XC_DBG("Test if file '%s' available: %s", filename,
            ret != -1 ? "OK":strerror(errno));
    return ret != -1;
}

bool xc::fs::search(xc::string& fn, const char* name, const char* path, bool recursive)
{
    //TODO: recursive
    xc::string fp(path);
    fp += '/';
    fp += name;
    if (avail(fp)) {
        fn = fp;
        return true;
    }

    return false;
}





