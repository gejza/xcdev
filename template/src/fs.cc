/*
   File name:  fs.cc
   Date:       2010/03/26 22:56
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <xc/fs.h>
#include <xc/text.h>
#include "xc/template/fs.h"
#include "xc/template/error.h"
#include "xc/template/scan_html.h"

xc::templ::files_t::files_t(const char* root)
    : _root(root)
{
}

xc::templ::files_t::~files_t()
{
}

void xc::templ::files_t::add_path(const char* path) {
    LOG_INFO(1, "Add '%s' to template search path.", path);
    _paths.add(path);
}

xc::templ::scanner_t* xc::templ::files_t::open(const char* name, bool all)
{
    if (xc::fs::avail(_root.c_str(), name))
        return new xc::templ::html_file_t((_root + "/" + name).c_str());
    if (all) {
        xc::string fn = _paths.search(name);
        if (!fn.empty())
            return new xc::templ::html_file_t(fn.c_str());
    }

    //ERROR(xc::templ::error_t, xc::format("File %s not found.", name));
    ERROR(xc::error_t, "File %s not found.", name);
    return NULL;
}



