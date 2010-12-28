/*
   File name:  test.cc
   Date:       2010/12/13 07:07
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdint.h>
#include <iostream>
#include <xc/log.h>
#include <xc/error.h>


#include <xc/resource/env.h>
//#include "pogen.h"

#if 0
void test_make_alias(xc::CBMake_t& cb)
{
    cb.alias("login", xc::CS, "prihlaseni");
}

void test_make()
{
    xc::CBMake_t cb;
    cb.generate_pot("test.pot");
    cb.set_default(xc::CS);
    test_make_alias(cb);
    //xc::Id_t id = cb.string("Hello World!!!");
    //std::cout << id << std::endl;
    //cb.string(id, xc::CS, "Ahoj svete!!!");
}

void test_read()
{
    //xc::CB_t cb;
    //std::cout << cb.string(1, xc::CS) << std::endl;
    //std::cout << cb.string(1, xc::EN) << std::endl;
    //std::cout << cb.string(1, xc::SK) << std::endl;*/
}
#endif

int xc_test_main(int argc, const char* argv[])
{
    try {
        xc::resource::Env_t env;
		env.get();
    } catch (libconfig::ParseException& e) {
        std::cerr << /*e.getFile() << ":" <<*/ e.getLine() << ": " << e.getError() << std::endl;
        throw;
    }
    //xc::POFile_t po("cs.po");
    //po.read();
    /*xc::POGen_t po("test.pot");
    po.add("Test");
    po.add("Jedna", "dva");
    xc::POGen_t::Msg_t msg("Reference");
    msg.addref("../test.xml", 23);
    msg.addref("../test.xml", 28);
    po.add(msg);*/
    return 0;
}





