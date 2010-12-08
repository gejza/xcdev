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

#include "pogen.h"
#include "serialize.h"

void test_serialize()
{
    xc::Serialize_t out;
    xc::Serialize_t out2;
    xc::Serialize_t out3;
    //out.write("include", cb.include);
    //out.write("script", cb.script);
    out3.write("t1");
    out3.write("t1");
    out3.write("t1");
    out2.write("class", "test2");
    out2.write("sub", out3);
    out2.write("meth", "metoda");
    out.write("method", "test");
    out.write("obj2", out2);
    out.write("obj3", out3);
    //out << templ.call;

    xc::Unserialize_t ser(out.str());
    xc::dump(ser);
}

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
    xc::CB_t cb;
    //std::cout << cb.string(1, xc::CS) << std::endl;
    //std::cout << cb.string(1, xc::EN) << std::endl;
    //std::cout << cb.string(1, xc::SK) << std::endl;*/
}

//int xc_test_main(int argc, const char* argv[])
//int main(int argc, const char* argv[])
int main()
{
    test_serialize();

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





