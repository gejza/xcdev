/*
   File name:  fd.h
   Date:       2010/03/25 10:26
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/

#ifndef _XC_FD_H_
#define _XC_FD_H_
#pragma once

#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#include <xc/string.h>

namespace xc {

/**
 * @brief fd_t
 */
class fd_t
{
public:

    /**
     * Default constructor
     */
    fd_t();

    /**
     * Constructor
     */
    fd_t(const char *filename, int flags = O_RDONLY, mode_t mode = 0);

    /**
     * Copy constructor
     */
    fd_t(const fd_t& fd);

    /**
     * Destructor
     */
    ~fd_t();

    void open(const char *filename, int flags = O_RDONLY, mode_t mode = 0);
    void close();

    int fd() {
        return _fd;
    }

    void write(const void* data, size_t size);

    void write(const xc::string& str) {
        this->write(str.data(), str.size());
    }

    bool lock(const struct ::flock& lck);

    void chown(const char* user, const char* group);
    void chown(uid_t uid, gid_t gid);
private:
    fd_t& operator=(const fd_t& other);

    int _fd;
    xc::string _filename;
};


} // namespace xc

#endif // _XC_FD_H_
/* end of fd.h */

