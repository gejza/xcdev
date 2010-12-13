/*
   File name:  compile.cc
   Date:       2010/12/13 07:45
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <xc/log.h>
#include <xc/error.h>

#include <stdint.h>
#include <iostream>

#include "../src/cb.h"
#include "../src/cbmake.h"

int xc_test_main(int argc, const char* argv[])
{
    xc::CBMake_t cb;
    //mfis::reg::Build_t b(argv[1]);
    cb.string("Error", "chyba", xc::CS);
    cb.string("Number", "cislo", xc::CS);
    for (int i=0; i < argc; i++)
    {
        std::cout << "Load file " << argv[i] << std::endl;
        cb.load_xml(argv[i]);
    }
    return 0;
}





