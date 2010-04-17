/*
   File name:  config.cc
   Date:       2010/03/17 03:01
   Subversion: $Id$
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <xc/version.h>
#include <xc/log.h>
#include <xc/config.h>
#include <xc/error.h>

int xc_test_main(int argc, const char* argv[])
{
    if (argc < 1) {
        fprintf(stderr, _("Usage: %s <file>\n"), argv[0]);
        return 1;
    }
        
    xc::config::load(argv[0]);

    const char* msg = xc::config::get("test", "Message");
    printf("%s\n", msg);
    return 0;
}




