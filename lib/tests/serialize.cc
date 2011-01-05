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

struct Test
{
	int a,b,c;
	std::string n;

	Test()
	{
		a = 1;
		b = 2;
		c = 3;
		n = "test";
	}
};

typedef std::map<std::string, Test> TestMap;
typedef std::vector<TestMap> TestList;

void serialize(xc::buffer_t& out, const std::string& key, const Test& value) {
	xc::serialize_t obj;
	obj.add("a", value.a);
	obj.add("b", value.b);
	obj.add("c", value.c);
	obj.add("n", value.n);
	serialize(out, key, obj);
}

int xc_test_main(int argc, const char* argv[])
{
	TestList test;
	TestMap tm;
	tm.insert(TestMap::value_type("aaa", Test()));
	tm.insert(TestMap::value_type("bbb", Test()));
	tm.insert(TestMap::value_type("ccc", Test()));
	test.push_back(tm);
	test.push_back(tm);
	test.push_back(tm);
	xc::buffer_t out = xc::serialize(test);
    xc::dump(xc::chunk(out));
    /*xc::serial_t out;
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
    xc::dump(ser);*/
    return 0;
}





