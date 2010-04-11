/*
   File name:  gc.cc
   Date:       2010/03/25 01:22
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <xc/log.h>
#include <xc/error.h>
#include <xc/pool.h>
#include <xc/debug.h>

void test_pool()
{
    xc::pool_t pool;
    XC_TRACE("test");
    for (int i=0;i <400;i++)
        pool.alloc(20);
    LOG_INFO(2, "strdup test: %s", pool.strdup("Hello world!!"));
    LOG_INFO(3, "Pool allocated as %zd/%zd", pool.used_size(), pool.alloc_size());
}

int xc_test_main(int argc, const char* argv[])
{
    test_pool();
    return 0;
}
