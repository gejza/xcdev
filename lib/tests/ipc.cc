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

#include <xc/log.h>
#include <xc/error.h>
#include <xc/mutex.h>

static xc::mutex_t m;

void test_lock()
{
    xc::lock_t l(m);
    printf("Lock in\n");
    ::sleep(3);
    printf("Lock out\n");
}

int xc_test_main(int argc, const char* argv[])
{
    ::fork();
    for (int i=0;i < 5;i++) {
        test_lock();
    }
    return 0;
}





