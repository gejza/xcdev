/*
   File name:  fs.h
   Date:       2010/03/26 21:18
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_TEMPLATE_FS_H_
#define _XC_TEMPLATE_FS_H_
#pragma once

#include <set>

#include <xc/paths.h>

namespace xc {
namespace templ {

// forward
class scanner_t;
class page_t;

/**
 * @short files_t
 */
class files_t
{
public:

    /**
     * Default constructor
     * @param root Root directory
     */
    files_t(const char* root);

    /**
     * Destructor
     */
    ~files_t();

    /** 
     * @short Add search path
     * @param path Directory
     */
    void add_path(const char* path);

    scanner_t* open(const char* name, bool all = false);

private:
    files_t(const files_t&);
    files_t& operator=(const files_t&);

    xc::string _root;
    xc::paths_t _paths;
};


} // namespace templ
} // namespace xc

#endif // _XC_TEMPLATE_FS_H_
/* end of fs.h */

