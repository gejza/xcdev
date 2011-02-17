/*
   File name:  ipc.cc
   Date:       2011/02/16 14:46
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <sys/types.h>
#include <sys/wait.h>

#include <xc/log.h>
#include <xc/error.h>
#include <xc/mutex.h>
#include <xc/ipc.h>

void test_lock(xc::mutex_t& m, int& n)
{
    xc::lock_t<xc::mutex_t> l(m);
    LOG_INFO(3, "Lock in %d", n);
    ::sleep(2);
    n++;
    LOG_INFO(3, "Lock out %d", n);
}

int xc_test_main(int argc, const char* argv[])
{
    xc::debug::debug_show_pid(true);
    xc::mutex_t m;
    xc::shpool_t pool(16);
    int* n = pool.alloc<int>(true);

    pid_t p = ::fork();
    for (int i=0;i < 2;i++) {
        test_lock(m, *n);
    }
    int status;
    if (p) {
        ::waitpid(p, &status, 0);
        pool.free(n);
    }
    return 0;
}





