/*
   File name:  fd.cc
   Date:       2010/03/25 12:50
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <errno.h>
#include <string.h>

#include <xc/system.h>

#include "xc/fd.h"
#include "xc/error.h"

xc::fd_t::fd_t() : _fd(-1)
{
}
    
xc::fd_t::fd_t(const char* filename, int flags, mode_t mode) : _fd(-1)
{
    this->open(filename, flags, mode);
}

xc::fd_t::fd_t(const fd_t& fd)
    : _fd(-1), _filename(fd._filename)
{
    if (fd._fd >= 0) {
        XC_DBG("Copy fd %d = `%s'", fd._fd, _filename.c_str());
        _fd = TEMP_FAILURE_RETRY(::dup(fd._fd));
        if (_fd == -1) {
            ERROR(xc::error_t, "Can't copy fd %d",
                            fd._fd, strerror(errno));
        }
    }
}


xc::fd_t::~fd_t()
{
    this->close();
}


void xc::fd_t::open(const char* filename, int flags, mode_t mode)
{
    XC_DBG("Open fd from `%s'", filename);
    _fd = TEMP_FAILURE_RETRY(::open(filename, flags, mode));
    if (_fd == -1) {
        ERROR(xc::error_t, "Can't open file %s for writing: %s",
                        filename, strerror(errno));
    }
    _filename = filename;
}

void xc::fd_t::close()
{
    if (_fd > 2) {
        XC_DBG("Close fd=%d (%s)", _fd, _filename.c_str());
        if (TEMP_FAILURE_RETRY(::close(_fd)) == -1) {
            ERROR(xc::error_t, "Can't close(fd = %d): %s",
                    _fd, strerror(errno));
        }
        _fd = -1;
        _filename.clear();
    }
}

void xc::fd_t::write(const void* data, size_t size)
{
    XC_DBG("Write %zd bytes to fd=%d (%s)", size, _fd, _filename.c_str());
    while (size) {
        // write as much as you can
        ssize_t n = TEMP_FAILURE_RETRY(::write(_fd, data, size));
        if (n == -1) {
            ERROR(xc::error_t, "Can't write to file `%s': %s",
                            _filename.c_str(), strerror(errno));
        }

        data = reinterpret_cast<const char*>(data) + n;
        size -= n;
    }
}

bool xc::fd_t::lock(const flock& lck)
{
    XC_DBG("Lock fd=%d (%s)", _fd, _filename.c_str());
    if (TEMP_FAILURE_RETRY(::fcntl(_fd, F_SETLK, &lck)) == -1) {
        // if already locked, throw error
        if ((errno == EAGAIN) || (errno == EACCES)) {
            LOG_WARN(1, "File %s already locked", _filename.c_str());
            return false;
        }

        ERROR(xc::error_t, "Can't acquire lock for fd=%d (%s): %s",
                            _fd, _filename.c_str(), strerror(errno));
    }

    return true;
}

void xc::fd_t::chown(const char* user, const char* group)
{
    this->chown(xc::system::get_uid(user), xc::system::get_gid(group));
}

void xc::fd_t::chown(uid_t uid, gid_t gid)
{
    if (TEMP_FAILURE_RETRY(::fchown(_fd, uid, gid)) == -1) {
        ERROR(xc::error_t, "Unable to change owner to %d:%d for fd=%d (%s).",
                int(uid), int(gid), _fd, _filename.c_str());
    }
}

