/*
   File name:  paths.h
   Date:       2010/03/26 22:49
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_PATHS_H_
#define _XC_PATHS_H_
#pragma once

#include <vector>
#include "xc/string.h"

namespace xc {

/**
 * @short paths_t
 */
class paths_t
{
public:

    struct path_t
    {
        xc::string path;
        bool recursive;
    };

    /**
     * Default constructor
     */
    paths_t(const char** def = 0x0);

    /**
     * Destructor
     */
    ~paths_t();

    /** 
     * @short Add search path
     * @param path Directory
     * @param recursive 
     */
    void add(const char* path, bool recursive = false) {
        path_t p = { path, recursive };
        _paths.push_back(p);
    }

    /** 
     * @short 
     * @param name 
     * @return 
     */
    xc::string search(const char* name);

private:
    paths_t(const paths_t&);
    paths_t& operator=(const paths_t&);

    typedef std::vector<path_t> _paths_t;

    _paths_t _paths;
    const char** _def;
};


} // namespace xc

#endif // _XC_PATHS_H_
/* end of paths.h */

