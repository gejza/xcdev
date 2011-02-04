/*
   File name:  test.cc
   Date:       2010/12/30 21:36
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>
#include <unistd.h>

#include <xc/test.h>
#include <xc/error.h>

#include "include/xc/rd/out_cdb.h"
#include "include/xc/rd/cdb.h"

using namespace xc::test;

#define OUT(msg...) xc::test::printf(msg)
#undef ERROR
#define ERROR(msg...) throw xc::error_t(msg)

void test_output(xc::rd::Output_t& out)
{
	//out.add(1,
	out.insert(1, "aaa", "bbb");
	out.insert(2, "aaa", "ccc");
}

void test_lookup(xc::rd::Lookup_t& look)
{
	xc::data_t val;
	if (look.lookup(1, "aaa", val))
		OUT("Value: %s\n", val.data());
	else
		ERROR("Value not defined.");
	if (look.lookup(2, "aaab", val))
		OUT("Value: %s\n", val.data());
	else
		ERROR("Value not defined.");
}

void test_output(const char* filename)
{
	xc::rd::CDBMake_t out(filename);
	test_output(out);
}

void test_lookup(const char* filename)
{
	//TODO: tmp file
	xc::rd::ConstDB_t look(filename);
	test_lookup(look);
}

int xc_test_main(int argc, const char* argv[])
{
	(void)argc;
	(void)argv;
	const char* fn = "/tmp/test.cdb";
	OUT("Test cdb file %s\n", fn);
	test_output(fn);
	test_lookup(fn);
	::unlink(fn);
    return 0;
}





