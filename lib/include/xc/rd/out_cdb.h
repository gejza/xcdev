/*
   File name:  out_cdb.h
   Date:       2010/12/18 15:11
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_RD_OUT_CDB_H_
#define _XC_RD_OUT_CDB_H_
#pragma once

#include <xc/fd.h>
#include <cdb.h>

#include "output.h"

namespace xc {
namespace rd {

class CDBMake_t : public Output_t
{
public:
    CDBMake_t(const char* fn);
    virtual ~CDBMake_t();

    virtual void insert(const xc::rd::ns_t ns, const xc::data_t& key,
			const xc::data_t& value);
private:
    xc::fd_t _fd;
    struct cdb_make _db;
};

} // namespace rd
} // namespace xc


#endif // _XC_RD_OUT_CDB_H_
/* end of out_cdb.h */

