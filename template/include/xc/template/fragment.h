/*
   File name:  fragment.h
   Date:       2010/04/20 01:42
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_FRAGMENT_H_
#define _XC_FRAGMENT_H_
#pragma once

#include <map>
#include <list>
#include <xc/string.h>

namespace xc {
namespace templ {

/**
 * @short frag_t
 */
class frag_t
{
public:
    typedef std::list<frag_t> list_t;

    struct value_t {
        xc::string str;
        const char* c_str() {
            return str.c_str();
        }
    };
    /**
     * Default constructor
     */
    frag_t();

    /**
     * Destructor
     */
    ~frag_t();

    void set(const xc::string& name, const xc::string& value);
    frag_t& create_frag(const xc::string& name);

    const value_t& get_value(const xc::string& name) const;
    const list_t& get_frag(const xc::string& name) const;
private:
    //frag_t(const frag_t&);
    //frag_t& operator=(const frag_t&);

    typedef std::map<xc::string, value_t> _value_map_t;
    typedef std::map<xc::string, list_t> _frag_map_t;

    _value_map_t _values;
    _frag_map_t _frags;
};

typedef frag_t::list_t frag_list_t;

} // namespace templ
} // namespace xc

#endif // _XC_FRAGMENT_H_
/* end of fragment.h */

