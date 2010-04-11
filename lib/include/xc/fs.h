/*
   File name:  fs.h
   Date:       2010/03/25 14:04
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_FS_H_
#define _XC_FS_H_
#pragma once

#include <xc/string.h>

namespace xc {
namespace fs {

xc::string eval_path(const char* path);

inline xc::string eval_path(const xc::string& path) {
    return eval_path(path.c_str());
}

bool avail(const char* path, const char* name);
bool avail(const char* filename);
inline bool avail(const xc::string& filename) {
    return avail(filename.c_str());
}

bool search(xc::string& fn, const char* name, const char* path, bool recursive = false);

} // namespace fs
} // namespace xc

#include "file.h"
#include "fd.h"
#include "dir.h"

#endif // _XC_FS_H_
/* end of fs.h */

