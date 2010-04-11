/*
   File name:  version.cc
   Date:       2010/03/30 13:11
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xc/version.h"

#ifdef DEBUG
#define INFO_STRING "XCLib " XC_VERSION " (DEBUG)"
#else
#define INFO_STRING "XCLib " XC_VERSION
#endif

#define BUILT_STRING "Svn: " SVN_VERSION " (built: " __DATE__ " " __TIME__ ") "
#define FULL_INFO_STRING INFO_STRING "\n" BUILT_STRING "\nCopyright (c) 2010 Milan Dunghubel\n"

const char* xc::get_version()
{
    return XC_VERSION;
}

const char* xc::get_built_info()
{
    return BUILT_STRING;
}

const char* xc::get_info()
{
    return INFO_STRING;
}

const char* xc::get_full_info()
{
    return FULL_INFO_STRING;
}


