/*
   File name:  regcomp.cc
   Date:       2010/12/03 14:07
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan.dunghubel@gmail.com>

   Copyright (C) 2010 Milan Dunghubel <milan.dunghubel@gmail.com>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>

#include "build.h"

void parse_xml(const char* fn, mfis::reg::Build_t& build);

int main(int argc, const char* argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <db>\n", argv[0]);
        return 1;
    }

    mfis::reg::Build_t b(argv[1]);
    for (int i=2; i < argc; i++)
    {
        parse_xml(argv[i], b);
    }
    return 0;
}


