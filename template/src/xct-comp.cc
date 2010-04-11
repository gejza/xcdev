/*
   File name:  xct-comp.cc
   Date:       2010/03/27 01:42
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>
#include <string.h>

#include <xc/config.h>
#include <xc/log.h>
#include <xc/text.h>
#include <xc/template/scan_html.h>
#include <xc/template/preprocess.h>
#include <xc/template/debug.h>
#include <xc/template/error.h>
#include <xc/template/fs.h>

int main(int argc, char* argv[])
{
    xc::templ::files_t fs(".");
    xc::templ::dump_format_t fmt = xc::templ::DF_LIST;
    FILE* out = stdout;

    int opt;
    while ((opt = ::getopt(argc, argv, "dM:cI:")) != -1) {
           switch (opt) {
           case 'I':
               fs.add_path(optarg);
               break;
           case 'd':
                xc::debug::debug_enable();
               break;
           case 'M':
               xc::log::add_stderr(optarg);
               break;
           default: /* '?' */
               ::fprintf(stderr, "Usage: %s [-c]  [-d] [-M LOGMASK] <FILE...>\n",
                       argv[0]);
               ::exit(EXIT_FAILURE);
           }
    }

    if (optind >= argc) {
        ::fprintf(stderr, "Usage: %s FILES...\n", argv[0]);
        ::exit(EXIT_FAILURE);
    }

    for (int i=optind; i < argc; i++) {
        fs.open(argv[i], true);
        //xc::templ::html_file_t hf(strcmp(argv[i], "-") ? argv[i]:"/dev/stdin");
        //xc::templ::dump(out, hf, fmt);
    }

    return EXIT_SUCCESS;
}



