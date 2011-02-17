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
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include "xc/mutex.h"
#include "xc/error.h"

#ifdef _SEM_SEMUN_UNDEFINED  // see sys/sem.h
union semun {
    int              val;    // value for SETVAL
    struct semid_ds *buf;    // buffer for IPC_STAT, IPC_SET
    unsigned short  *array;  // array for GETALL, SETALL
    struct seminfo  *__buf;  // buffer for IPC_INFO (Linux-specific)
};
#endif // _SEM_SEMUN_UNDEFINED

xc::mutex_t::mutex_t()
    : _semsetid(::semget(IPC_PRIVATE, 1, IPC_CREAT | 0600)), _owner(::getpid())
{
    // init semaphore
    union semun arg;
    arg.val = 1;
    if ((::semctl(_semsetid, 0, SETVAL, arg)) < 0) {
        ERROR(xc::error_t, "semctl: %s", strerror(errno));
    }
}

xc::mutex_t::~mutex_t()
{
    if (_owner == ::getpid()) {
        if (::semctl(_semsetid, 0, IPC_RMID, 0)) {
            ERROR(xc::error_t, "semctl: %s", strerror(errno));
        }
    }
}

void xc::mutex_t::lock()
{
    // prepare structure for lock
    struct sembuf op = { 0, -1, SEM_UNDO};

    // lock
    if (TEMP_FAILURE_RETRY(::semop(_semsetid, &op, 1)) < 0) {
        ERROR(xc::error_t, "semop: %s", strerror(errno));
    }
    XC_DBG("Mutex locked.");
}

void xc::mutex_t::unlock()
{
    // prepare structure for unlock
    struct sembuf op = { 0, +1, SEM_UNDO};

    // unlock
    if (TEMP_FAILURE_RETRY(::semop(_semsetid, &op, 1)) < 0) {
        ERROR(xc::error_t, "semop: %s", strerror(errno));
    }
    XC_DBG("Mutex unlocked.");
}

///////////////////////////////////////
// pmutex_t
xc::pmutex_t::pmutex_t()
{
	if (TEMP_FAILURE_RETRY(::pthread_mutex_init(&this->_mutex, NULL)) != 0) {
        ERROR(xc::error_t, "Can't init mutex %s", strerror(errno));
	}
}

xc::pmutex_t::~pmutex_t()
{
	if (TEMP_FAILURE_RETRY(::pthread_mutex_destroy(&this->_mutex)) != 0) {
        ERROR(xc::error_t, "Can't destroy mutex %s", strerror(errno));
	}
}

void xc::pmutex_t::lock()
{
	if (TEMP_FAILURE_RETRY(::pthread_mutex_lock(&this->_mutex)) != 0) {
        ERROR(xc::error_t, "Can't lock mutex %s", strerror(errno));
	}
    XC_DBG("Mutex locked.");
}

void xc::pmutex_t::unlock()
{
	if (TEMP_FAILURE_RETRY(::pthread_mutex_unlock(&this->_mutex)) != 0) {
        ERROR(xc::error_t, "Can't unlock mutex %s", strerror(errno));
	}
    XC_DBG("Mutex unlocked.");
}




