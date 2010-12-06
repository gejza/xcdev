/*
   File name:  fd.cc
   Date:       2010/12/05 23:04
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "fd.h"

namespace {
    int open_read(const char* fn)
    {
        int fd = ::open(fn, O_RDONLY);
        if (fd < 0) {
            fprintf(stderr, "Failed open %s for read.\n", fn);
            abort();
        }
        return fd;
    }

    int open_write(const char* fn)
    {
        mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
        int flags = O_RDWR|O_CREAT;
        int fd = ::open(fn, flags, mode);
        if (fd < 0) {
            fprintf(stderr, "Failed open %s for write.\n", fn);
            abort();
        }
        return fd;
    }
}

FD_t::FD_t(const char* fn, bool create)
    : _fd(create ? ::open_write(fn) : ::open_read(fn))
{
}

FD_t::~FD_t()
{
    int ret = ::close(this->_fd);
}




