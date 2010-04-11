/*
   File name:  system.cc
   Date:       2010/03/25 12:41
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <stdio.h>

#include <xc/system.h>
#include <xc/log.h>

void test_pid()
{
    printf("--- %s ---\n", __FUNCTION__);
    sleep(10);
}

int xc_test_main(int argc, const char* argv[])
{
    xc::log::add_file("system.log", "ALL");
    //xc::system::set_user("nobody", "nogroup");
    printf("Forking to background...\n");
    xc::system::daemonize();
    xc::system::pidfile_t pid("test.pid");
    printf("Now i hidden...\n");
    sleep(10);
    return 0;
}



