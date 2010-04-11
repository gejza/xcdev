/*
   File name:  db.h
   Date:       2010/03/26 21:17
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_TEMPLATE_DB_H_
#define _XC_TEMPLATE_DB_H_
#pragma once

#include <cdb.h>

#include <xc/fd.h>

namespace xc {
namespace templ {

/**
 * @short Pages database
 */
class db_t
{
public:

    /**
     * Default constructor
     */
    db_t(const char* filename);

    /**
     * Destructor
     */
    ~db_t();

private:
    db_t(const db_t&);
    db_t& operator=(const db_t&);

    fd_t _fd;
    ::cdb _db;
};

/**
 * @short Database creator
 */
class dbw_t
{
public:

    /**
     * Default constructor
     */
    dbw_t(const char* filename);

    /**
     * Destructor
     */
    ~dbw_t();

private:
    dbw_t(const dbw_t&);
    dbw_t& operator=(const dbw_t&);

    fd_t _fd;
    ::cdb_make _db;
};


} // namespace templ
} // namespace xc

#endif // _XC_TEMPLATE_DB_H_
/* end of db.h */

