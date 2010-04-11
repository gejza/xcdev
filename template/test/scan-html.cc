/*
   File name:  parse-html.cc
   Date:       2010/03/26 11:45
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <memory>

#include <xc/config.h>
#include <xc/log.h>
#include <xc/text.h>
#include "xc/template/fs.h"
#include "xc/template/scan_html.h"
#include "xc/template/preprocess.h"
#include "xc/template/debug.h"
#include "xc/template/error.h"

using namespace xc::templ;

int xc_test_main(int argc, const char* argv[])
{
    int style = 1;
    xc::templ::dump_format_t fmt = xc::templ::DF_LIST;
    xc::templ::files_t fs(".");
    fs.add_path("test");
    for (int i=0; i < argc; i++) {
        std::auto_ptr<xc::templ::scanner_t> s(fs.open(argv[i]));
        xc::templ::dump(stdout, *s, fmt);

    }
    //html_file_t hf("/dev/stdin");

    /*preproc_t f1(hf);
    preproc_t f(static_cast<xc::templ::scanner_t&>(f1));
    while (xc::templ::scanner_t::term_t t = f.read()) {
        if (argc)
            output_color(t);
        else
            output_desc(t);
    }*/

    return 0;
}



