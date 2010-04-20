/*
   File name:  fragment.cc
   Date:       2010/04/20 01:57
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <xc/config.h>
#include <xc/log.h>
#include <xc/error.h>
#include <xc/text.h>


#include "xc/template/fragment.h"


xc::templ::frag_t::frag_t()
{
}

xc::templ::frag_t::~frag_t()
{
}

const xc::templ::frag_t::value_t& xc::templ::frag_t::get_value(const xc::string& name) const
{
    _value_map_t::const_iterator i = _values.find(name);
    if (i != _values.end())
        return i->second;
    static value_t empty;
    return empty;
}

const xc::templ::frag_t::list_t& xc::templ::frag_t::get_frag(const xc::string& name) const
{
    _frag_map_t::const_iterator i = _frags.find(name);
    if (i != _frags.end())
        return i->second;
    static list_t empty;
    return empty;
}

xc::templ::frag_t& xc::templ::frag_t::create_frag(const xc::string& name)
{
    list_t& list = _frags[name];
    list.push_back(frag_t());
    return list.back();
}

void xc::templ::frag_t::set(const xc::string& name, const xc::string& value)
{
    _values[name].str = value;
}



