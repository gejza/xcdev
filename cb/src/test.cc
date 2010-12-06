/*
   File name:  test.cc
   Date:       2010/05/07 20:47
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdint.h>
#include <iostream>
/*#include <xc/log.h>
#include <xc/error.h>
*/
#include <xc/cbmake.h>
#include <xc/cb.h>

void test_make_hooks(xc::CBMake_t& cb)
{
    cb.hook("exit");
}

void test_make_alias(xc::CBMake_t& cb)
{
    cb.alias("login", xc::CS, "prihlaseni");
}

//int xc_test_main(int argc, const char* argv[])
//int main(int argc, const char* argv[])
int main()
{
    /*xc::POFile_t po("cs.po");
    po.read();*/


    {
        xc::CBMake_t cb;
        cb.generate_pot("test.pot");
        cb.set_default(xc::CS);
        test_make_alias(cb);
        /*xc::Id_t id = cb.string("Hello World!!!");
        std::cout << id << std::endl;
        cb.string(id, xc::CS, "Ahoj svete!!!");*/
    }
    {
        xc::CB_t cb;
        /*std::cout << cb.string(1, xc::CS) << std::endl;
        std::cout << cb.string(1, xc::EN) << std::endl;
        std::cout << cb.string(1, xc::SK) << std::endl;*/
    }

    return 0;
}





