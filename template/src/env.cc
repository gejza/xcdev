/*
   File name:  env.cc
   Date:       2010/03/30 14:14
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xc/template/env.h"
#include "xc/template/page.h"
#include "xc/template/error.h"
#include "xc/template/source.h"
#include "xc/template/fs.h"
#include "xc/template/compile.h"

namespace {
    class file_source_t : public xc::templ::source_t
    {
    public:
        file_source_t(const char* root)
            : _files(root)
        {
        }

        virtual ~file_source_t() {}

        virtual xc::templ::page_t* get_page(const char* name) {
            //_files.open((xc::string(name) + ".html").c_str());
            xc::templ::compiler_t c(_files);
            return c.compile((xc::string(name) + ".html").c_str());

        }
    private:
        xc::templ::files_t _files;
    };
}

xc::templ::env_t::env_t(const char* root)
{
    _sources.push_back(new file_source_t(root));
}

xc::templ::env_t::~env_t()
{
}

xc::templ::page_t* xc::templ::env_t::get_page(const char* name) const
{
    // find from cache

    for (_sources_t::const_iterator i = _sources.begin(); i != _sources.end(); ++i) {
        page_t* page = (*i)->get_page(name);
        if (page)
            return page;
    }
    ERROR(xc::error_t, "Page '%s' not found.", name);
}


