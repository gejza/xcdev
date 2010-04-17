/*
   File name:  env.h
   Date:       2010/03/27 00:48
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_TEMPLATE_ENV_H_
#define _XC_TEMPLATE_ENV_H_
#pragma once

#include <vector>

#include "fs.h"
#include "db.h"

namespace xc {
namespace templ {

class source_t;

/**
 * @short env_t
 */
class env_t
{
public:

    /**
     * Default constructor
     */
    env_t(const char* root);

    /**
     * Destructor
     */
    ~env_t();

    page_t* get_page(const char* name) const;

    
private:
    env_t(const env_t&);
    env_t& operator=(const env_t&);

    //files_t _files;
    //db_t _db;
    typedef std::vector<source_t*> _sources_t;
    mutable _sources_t _sources;
};


} // namespace templ
} // namespace xc

#endif // _XC_TEMPLATE_ENV_H_
/* end of env.h */

