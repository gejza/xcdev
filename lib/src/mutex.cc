/*
   File name:  mutex.cc
   Date:       2011/02/16 01:10
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan@mfis.cz>

   Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <errno.h>
#include <string.h>

#include "xc/mutex.h"
#include "xc/error.h"

xc::mutex_t::mutex_t()
{
	if (TEMP_FAILURE_RETRY(::pthread_mutex_init(&this->_mutex, NULL)) != 0) {
        ERROR(xc::error_t, "Can't init mutex %s", strerror(errno));
	}
}

xc::mutex_t::~mutex_t()
{
	if (TEMP_FAILURE_RETRY(::pthread_mutex_destroy(&this->_mutex)) != 0) {
        ERROR(xc::error_t, "Can't destroy mutex %s", strerror(errno));
	}
}

void xc::mutex_t::lock()
{
	if (TEMP_FAILURE_RETRY(::pthread_mutex_lock(&this->_mutex)) != 0) {
        ERROR(xc::error_t, "Can't lock mutex %s", strerror(errno));
	}
}

void xc::mutex_t::unlock()
{
	if (TEMP_FAILURE_RETRY(::pthread_mutex_unlock(&this->_mutex)) != 0) {
        ERROR(xc::error_t, "Can't unlock mutex %s", strerror(errno));
	}
}




