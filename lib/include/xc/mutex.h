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
	pthread_mutex_t _mutex;
};

class lock_t
{
public:
	lock_t(mutex_t& mutex)
		: _mutex(mutex)
	{
		_mutex.lock();
	}
	~lock_t()
	{
		_mutex.unlock();
	}
private:
	mutex_t& _mutex;
};

} // namespace xc

#endif // _XC_MUTEX_H_
/* end of mutex.h */

