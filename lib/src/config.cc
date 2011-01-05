/*
   File name:  config.cc
   Date:       2010/03/17 04:15
   Subversion: $Id$
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <memory>
#include <stdio.h>
#include <string.h>
#include <stdexcept>

#include "xc/log.h"
#include "xc/config.h"
#include "xc/file.h"
#include "xc/fs.h"

using namespace xc::config;

////////////////////////
// dsection
bool xc::config::dyn_section_t::ltstr_t::operator()(const char* s1, const char* s2) const
{
    return ::strcmp(s1, s2) < 0;
}

const char* xc::config::dyn_section_t::get(const char* name) const
{
    map_type::const_iterator i = _map.find(name);
    if (i == _map.end()) {
        ERROR(xc::config::error_t, "Value `%s' not found.", name);
    }
    XC_DBG("Get config value %s=%s", name, i->second);

    return i->second;
}

void xc::config::dyn_section_t::set(const char* name, const char* value)
{
    XC_DBG("Set config value %s=%s", name, value);
    _map.insert(map_type::value_type(_pool.strdup(name), _pool.strdup(value)));
}

namespace xc {
namespace config {

    void parse_ini_file(const char* fn);

} //namespace config
} // namespace xc

////////////////////////
// config
namespace {
    typedef std::map<std::string, section_t*> section_map_type;
    section_map_type _section_map;
}


const char* xc::config::get(const char* section, const char* name)
{
    return find_section(section).get(name);
}

xc::string xc::config::get_path(const char* section, const char* name)
{
    return xc::fs::eval_path(find_section(section).get(name));
}

const section_t& xc::config::find_section(const char* name)
{
    section_map_type::const_iterator i = _section_map.find(name);
    if (i == _section_map.end()) {
        LOG_ERROR(2, "Section `%s' not found.", name);
        throw 1;
    }

    return *i->second;
}

void xc::config::add_section(const char* name, section_t* sec)
{
    XC_DBG("Add config section %s", name);
    _section_map.insert(section_map_type::value_type(name, sec));
}

void xc::config::load(const char* path)
{
    LOG_INFO(2, "Load config from `%s'", path);
    parse_ini_file(path);
}



