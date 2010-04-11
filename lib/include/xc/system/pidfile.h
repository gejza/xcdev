/*
   File name:  pidfile.h
   Date:       2010/03/25 12:31
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_SYSTEM_PIDFILE_H_
#define _XC_SYSTEM_PIDFILE_H_
#pragma once

#include <xc/fd.h>
#include <xc/string.h>

namespace xc {
namespace system {

/**
 * @brief pidfile_t
 */
class pidfile_t
{
public:

    /**
     * Default constructor
     */
    pidfile_t(const char* filename);

    /**
     * Destructor
     */
    ~pidfile_t();

private:
    pidfile_t(const pidfile_t& other);
    pidfile_t& operator=(const pidfile_t& other);

    fd_t _fd;
    xc::string _filename;
};


} // namespace system
} // namespace xc

#endif // _XC_SYSTEM_PIDFILE_H_
/* end of pidfile.h */

