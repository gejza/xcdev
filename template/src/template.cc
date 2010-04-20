/*
   File name:  template.cc
   Date:       2010/03/30 14:12
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string.h>

#include <xc/config.h>
#include <xc/log.h>
#include <xc/error.h>
#include <xc/text.h>

#include "xc/template.h"
#include "xc/template/vm.h"

xc::templ::templ_t::templ_t(const xc::templ::env_t& env)
    : _env(env)
{
}

xc::templ::templ_t::~templ_t()
{
}

void xc::templ::templ_t::add_page(char const* source)
{
    _pagelist.push_back(_env.get_page(source));
    LOG_INFO(2, "Add page '%s' to template.", source);
}

void xc::templ::templ_t::set_page(const char* name, const char* source)
{
    _page_list_t list;
    list.push_back(_env.get_page(source));
    _pages.insert(_page_map_t::value_type(name, list));
    LOG_INFO(2, "Set page '%s' as %s.", source, name);
}

void xc::templ::templ_t::add_page(const char* name, const char* source)
{
    _pages[name].push_back(_env.get_page(source));
    LOG_INFO(2, "Add page '%s' as %s.", source, name);
}

void xc::templ::templ_t::generate(xc::templ::output_t& out)
{
    this->process(_pagelist);
    LOG_INFO(1, "Flush template output");
    this->flush();
}

void xc::templ::templ_t::process(const xc::templ::templ_t::_page_list_t& list)
{
    for (_page_list_t::const_iterator i = list.begin(); i != list.end(); ++i) {
        vm_t::process(**i, _root);
    }
}

void xc::templ::templ_t::vm_output(const char* ptr, size_t data)
{
    fwrite(ptr, data, 1, stdout);
}

void xc::templ::templ_t::vm_debug(const char* ptr, size_t size)
{
    vm_output("<!-- ", 5);
    vm_output(ptr, size);
    vm_output(" -->", 4);
}

void xc::templ::templ_t::vm_page(const xc::string& name)
{
    if (name == "DEBUG") {
        xc::string str = xc::format("<style>.debug { font:12px courier; text-align:left; color:black; background-color: silver; } </style>"
            "<div class=\"debug\">\n"
        "<p>INFO3: Generate at vm.cc:55</p>\n"
        "<p>INFO3: Generate at vm.cc:55</p>\n"
        "<p>INFO3: Generate at vm.cc:55</p>\n"
        "<p style=\"color: red;\">WARN4: Generate at vm.cc:55</p>\n"
        "<p>INFO3: Generate at vm.cc:55</p>\n"
        "<p>INFO3: Generate at vm.cc:55</p>\n"
    "</div>\n");
        vm_output(str.data(), str.size());
        return;
    }

    // find in pages
    _page_map_t::const_iterator i = _pages.find(name);
    if (i == _pages.end()) {
        ERROR(xc::error_t, "Page value '%s' not found.", name.c_str());
    }
    this->process(i->second);
}

void xc::templ::templ_t::vm_dict(const xc::string& name)
{
    const char* t1 = "<span style=\"color: red;\">";
    const char* t2 = "</span>";
    vm_output(t1, strlen(t1));
    vm_output(name.data(), name.size());
    vm_output(t2, strlen(t2));
}

void xc::templ::templ_t::vm_value(const xc::string& name)
{
}

extern "C" {
    const char* xctemplate_version()
    {
        return "1.0 verze";
    }
}
