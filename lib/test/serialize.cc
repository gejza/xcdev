/*
   File name:  serialize.cc
   Date:       2010/12/12 22:47
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <xc/log.h>
#include <xc/error.h>
#include <xc/serialize.h>
#include <xc/unserialize.h>

int xc_test_main(int argc, const char* argv[])
{
    xc::serial_t out;
    xc::serial_t out2;
    xc::serial_t out3;
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

    xc::unserialize_t ser(out.str());
    xc::dump(ser);
    return 0;
}





