/*
   File name:  timer.cc
   Date:       2011/02/14 17:11
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <errno.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>

#include "xc/timer.h"
#include "xc/error.h"

xc::timer_t::timer_t()
    : _start(0), _finish(0)
{
}

xc::timer_t::~timer_t()
{
}

void xc::timer_t::start()
{
    this->_start = this->get_time();
}

xc::timer_t::value_type xc::timer_t::finish()
{
    this->_finish = this->get_time();
    return this->get();
}

xc::timer_t::value_type xc::timer_t::get()
{
    return this->_finish - this->_start;
}

xc::timer_t::value_type xc::timer_t::get_time()
{
    struct timeval t;
    struct timezone tz;
    //TODO: osetrit chybu
    if (TEMP_FAILURE_RETRY(::gettimeofday(&t, &tz)) < 0) {
        ERROR(xc::error_t, "Can't gettimeofday %s", strerror(errno));
    }
    return value_type(t.tv_sec) + value_type(t.tv_usec * 0.000001);
}




