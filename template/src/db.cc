/*
   File name:  db.cc
   Date:       2010/03/26 22:07
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <fcntl.h>

#include <xc/log.h>

#include "xc/template/db.h"

////////////////////////////////////
// db read
xc::templ::db_t::db_t(const char* filename)
    : _fd(filename)
{
    cdb_init(&_db, _fd.fd());
    LOG_INFO(2, "Open template db `%s'", filename);
}

xc::templ::db_t::~db_t()
{
    cdb_free(&_db);
}


////////////////////////////////////
// db write
xc::templ::dbw_t::dbw_t(const char* filename)
    : _fd(filename, O_WRONLY | O_CREAT | O_TRUNC,
            S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
{
    cdb_make_start(&_db, _fd.fd());
    LOG_INFO(2, "Open template db `%s' for write", filename);
    char val[31] = "ahoj";
    char key[64];
    for (int i=0; i < 200;i++) {
        cdb_make_add(&_db, key, sprintf(key, "%d", i),
                 val, 31);
    }
    /*
int cdb_make_exists(struct cdb_make *cdbmp,
                    const void *key, unsigned klen);
int cdb_make_find(struct cdb_make *cdbmp,
                  const void *key, unsigned klen,
                  enum cdb_put_mode mode);
int cdb_make_put(struct cdb_make *cdbmp,
                 const void *key, unsigned klen,
                 const void *val, unsigned vlen,
                 enum cdb_put_mode mode);
    */

}

xc::templ::dbw_t::~dbw_t()
{
    cdb_make_finish(&_db);
}



