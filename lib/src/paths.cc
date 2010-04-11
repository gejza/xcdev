/*
   File name:  paths.cc
   Date:       2010/03/26 22:52
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xc/config.h"
#include "xc/log.h"
#include "xc/error.h"
#include "xc/fs.h"
#include "xc/paths.h"

xc::paths_t::paths_t(const char** def)
    : _def(def)
{
}

xc::paths_t::~paths_t()
{
}

xc::string xc::paths_t::search(const char* name)
{
    xc::string ret;
    for (_paths_t::const_reverse_iterator p = _paths.rbegin(); p != _paths.rend(); --p) {
        if (fs::search(ret, name, p->path.c_str(), p->recursive))
            return ret;
    }
    if (_def) {
        for (int i=0; _def[i]; i++) {
            if (fs::search(ret, name, _def[i]))
                return ret;
        }
    }
    return ret;
}


