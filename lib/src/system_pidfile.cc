/*
   File name:  system_pidfile.cc
   Date:       2010/03/25 12:46
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include "xc/log.h"
#include "xc/text.h"
#include "xc/error.h"
#include "xc/system.h"

xc::system::pidfile_t::pidfile_t(const char* filename)
    : _filename(filename)
{
    LOG_INFO( 4, "Using pidfile `%s`", filename);
    _fd.open(filename, O_RDWR | O_CREAT, 0644);
    // write pid to pidfile
    _fd.write(xc::format("%d", getpid()));

    // initialise flock structure
    flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;

    if (!_fd.lock(lock))
        ERROR(xc::error_t, "Pidfile %s already locked", _filename.c_str());
}

xc::system::pidfile_t::~pidfile_t()
{
    LOG_INFO( 4, "Removing pidfile `%s'", _filename.c_str());
    // close pidfile
    _fd.close();

    // unlink pidfile
    if (TEMP_FAILURE_RETRY(::unlink(_filename.c_str())) == -1) {
        LOG_WARN( 4, "Can't unlink pidfile `%s': %s",
                _filename.c_str(), strerror(errno));
    }
}





