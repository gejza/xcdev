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

#include <xc/log.h>
#include <xc/error.h>
#include <xc/template/db.h>

int xc_test_main(int argc, const char* argv[])
{
    {
        xc::templ::dbw_t db("test.db");
    }

    {
        xc::templ::db_t db("test.db");
    }
    return 0;
}





