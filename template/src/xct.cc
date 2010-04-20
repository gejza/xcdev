/*
   File name:  xct.cc
   Date:       2010/03/30 12:48
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>
#include <iostream>

#include <xc/config.h>
#include <xc/log.h>
#include <xc/error.h>
#include <xc/stream.h>

#include <xc/template.h>

/*void show_usage(const char* progname)
{
    fprintf(stderr, _("Usage: %s \n"), progname);
}

void show_help()
{
    fprintf(stderr, _("  -V, --version\n"));
    fprintf(stderr, _("  -V                         Zobraz verzi\n"));
}*/

int main(int argc, const char* argv[])
{
    try {
        // set init
        xc::log::add_stderr("ALL");
        //xc::debug::debug_enable();

        // main
        xc::templ::env_t env("./test");
        xc::ostream_file_t of(stdout);
        xc::templ::html_output_t out(of);
        xc::templ::templ_t templ(env);
        templ.add_page("test_fragment");
        templ.root().create_frag("big").create_frag("test1").create_frag("aaa").set("title", "Helllo");
        //templ.set_page("main", "main");
        templ.generate(out);

    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        ::exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}



