/*
   File name:  pofile.h
   Date:       2010/12/06 00:35
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_POFILE_H_
#define _XC_POFILE_H_
#pragma once

#include <gettext-po.h>

namespace xc {

class POFile_t
{
public:
    POFile_t(const char* fn);
    virtual ~POFile_t();

    void read();
protected:
    POFile_t();

    po_file_t _file;
};

} // namespace xc

#endif // _XC_POFILE_H_
/* end of pofile.h */

