/*
   File name:  config.h
   Date:       2010/03/17 16:40
   Subversion: $Id$
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

/**
 * @example config.cc
 */
#ifndef _XC_CONFIG_H_
#define _XC_CONFIG_H_
#pragma once

#include <string>
#include <map>

#include <xc/pool.h>
#include <xc/string.h>
#include <xc/error.h>

namespace xc {
namespace config {

typedef ::xc::error_t error_t;
/*class error_t : public xc::error_t
{
public:
    error_t() {}
    virtual xc::string message() const {
        return "config error";
    }
};*/


class section_t
{
public:
    virtual ~section_t() {}

    /**
     * Hodnota konfigu
     * @param name Jmeno parametru
     * @return Hodnota
     */
    virtual const char* get(const char* name) const = 0;
};


class dyn_section_t : public section_t
{
public:
    struct ltstr_t
    {
        bool operator()(const char* s1, const char* s2) const;
    };

    typedef std::map<const char*, const char*, ltstr_t> map_type;
    
    virtual ~dyn_section_t() {}

    virtual const char* get(const char* name) const;

    virtual void set(const char* name, const char* value);
private:
    pool_t      _pool;
    map_type    _map;
};

/**
 * Hodnota konfigu
 * @param name Jmeno parametru
 * @return Hodnota
 */
const char* get(const char* section, const char* name);

/**
 * Hodnota konfigu
 * @param name Jmeno parametru
 * @return Hodnota
 */
template<typename Value_t>
Value_t get(const char* section, const char* name, Value_t def)
{
    try {
        return get(section, name);
    } catch (...) {
        return def;
    }
}

/**
 * Hodnota konfigu jako cesta
 * @param name Jmeno parametru
 * @return Hodnota
 */
string get_path(const char* section, const char* name);

/**
 * Metoda nelezne sekci, Pri neupechu vyhazuje vyjimku.
 * @param name Jmeno sekce
 * @return sekce
 */
const section_t& find_section(const char* name);

/**
 * Registrace sekce. Pri zruseni vola delete
 * @param name Jmeno sekce
 * @param sec Sekce
 */
void add_section(const char* name, section_t* sec);

/**
 * Nahraje konfig ze souboru v ini formatu
 * @param path Cesta k souboru
 */
void load(const char* path);

} //namespace config
} // namespace xc

#endif // _XC_CONFIG_H_
/* end of config.h */

