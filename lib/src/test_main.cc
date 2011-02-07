/*
   File name:  test_main.cc
   Date:       2010/03/25 08:07
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <unistd.h>

#include <xc/version.h>
#include <xc/log.h>
#include <xc/config.h>
#include <xc/text.h>
#include <xc/error.h>

#include "include/xc/test.h"

extern int xc_test_main(int argc, const char* argv[]);

////////////////////////////////////////////////
// Test Functions

void xc::test::printf(const char *fmt, ...) {
    va_list ap;
    va_start (ap, fmt);
	xc::test::vprintf(fmt, ap);
    va_end (ap);
}

void xc::test::vprintf(const char *fmt, va_list ap) {

	::vprintf(fmt, ap);
}

namespace {
	void print_version()
	{
		::fputs(xc::get_full_info(), stderr);
	}

	void print_usage(const char* name)
	{
		fprintf(stderr, "Usage: %s\n", name);
	}

	void print_help(const char* name)
	{
		print_usage(name);
	}
}

int main(int argc, char* const* argv)
{
    int opt;
    bool debug = false;
    const char* mask = "ALL";
    //TODO default parameters do ~/.xc

    while ((opt = getopt(argc, argv, "VdqM:")) != -1) {
        switch (opt) {
        case 'V':
               print_version();
               return EXIT_SUCCESS;
        case 'd':
            debug = true;
            break;
        case 'q':
            debug = false;
            mask = NULL;
            break;
        case 'M':
            mask = optarg;
            break;
        default: /* '?' */
               print_usage(argv[0]);
               exit(EXIT_FAILURE);
           }
    }

    if (mask) xc::log::add_stderr(mask);
    if (debug) xc::debug::debug_enable();

    xc::i18n("cs_CZ.UTF-8");

    try {
        return xc_test_main(argc-optind, (const char**)&argv[optind]);
    } catch (const xc::error_t& e) {
		xc::colored_t red(stderr, 31);
        std::cerr << e.message() << std::endl;
        return EXIT_FAILURE;
    } catch (const std::exception& e) {
		xc::colored_t red(stderr, 31);
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (...) {
		xc::colored_t red(stderr, 31);
        std::cerr << "Unknown exception" << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}

