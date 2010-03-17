/*
   File name:  log.cc
   Date:       2010/03/17 03:01
   Subversion: $Id$
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <xc/log.h>

int main(int argc, const char* argv[])
{
    LOG_INFO(3, "Info message %d", 5);
    LOG_WARN(4, "Warning message %d", 5);
    LOG_ERROR(1, "Error message %d", 5);
    LOG_FATAL("Fatal message %d", 5);
    return 0;
}



