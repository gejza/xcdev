/*
   File name:  reglookup.cc
   Date:       2010/12/20 03:30
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "StdAfx.h"

#include <xc/log.h>
#include <xc/error.h>

#include <xc/unserialize.h>

#include "../include/xc/rd/cdb.h"

int main(int argc, const char* argv[])
{
    xc::debug::debug_enable();
    xc::log::add_stderr("ALL");

    if (argc <= 3)
    {
        fprintf(stderr, "Usage: %s <db> <ns> <key>\n", argv[0]);
        return EXIT_FAILURE;
    }

    try {
		int ns = atoi(argv[2]);
		const char* key = argv[3];
        xc::rd::ConstDB_t lookup(argv[1]);
    	xc::data_t val;
		if (lookup.lookup(ns, key, val)) {
			xc::dump(xc::chunk(val));
		}

    } catch (const xc::error_t& e) {
        std::cerr << "Error: " << e.message() << std::endl;
        return EXIT_FAILURE;
    } catch (const std::exception& e) {
        std::cerr << "Std exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Unknown exception" << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}





