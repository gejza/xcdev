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

#include <libconfig.h>

#include <xc/log.h>

int main(int argc, const char* argv[])
{
    config_t cfg;
    config_init(&cfg);
    if (config_read_file(&cfg, "test.cfg") == CONFIG_FALSE) {
        LOG_FATAL("Failed read test.cfg: %s", config_error_text(&cfg));
    }
    LOG_INFO(2, "Read test.cfg");

    const char* msg = NULL;
    if (config_lookup_string(&cfg, "application.window.title", &msg) == CONFIG_TRUE) {
        printf("%s\n", msg);
    }


    config_destroy(&cfg);

    return 0;
}




