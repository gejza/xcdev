/*
   File name:  timer.cc
   Date:       2011/02/14 17:12
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <iostream>

#include <xc/log.h>
#include <xc/error.h>
#include <xc/timer.h>

int xc_test_main(int argc, const char* argv[])
{
    xc::timer_t t;
    t.start();
    ::sleep(1);
    t.finish();
    std::cout << "Time: " << t.get() << std::endl;

    return 0;
}





