/*
   File name:  mutex.h
   Date:       2011/02/16 01:04
   Subversion: $Id: $
   Author:     Milan Dunghubel <milan.dunghubel@gmail.com>

   Copyright (C) 2010 Milan Dunghubel <milan.dunghubel@gmail.com>
*/

#ifndef _XC_MUTEX_H_
#define _XC_MUTEX_H_
#pragma once

#include <pthread.h>

namespace xc {

class mutex_t
{
public:
	mutex_t();
	~mutex_t();
	void lock();
	void unlock();
private:
    mutex_t(const mutex_t &);
    mutex_t &operator=(const mutex_t &);

    int _semsetid;
    pid_t _owner;
};

class pmutex_t
{
public:
	pmutex_t();
	~pmutex_t();
	void lock();
	void unlock();
private:
    pmutex_t(const pmutex_t &);
    pmutex_t &operator=(const pmutex_t &);

	pthread_mutex_t _mutex;
};

template<typename object_t>
class lock_t
{
public:
	lock_t(object_t& object) : _object(object) { _object.lock(); }
	~lock_t() { _object.unlock(); }
private:
    lock_t(const lock_t &);
    lock_t &operator=(const lock_t &);

	object_t& _object;
};

} // namespace xc

#endif // _XC_MUTEX_H_
/* end of mutex.h */

