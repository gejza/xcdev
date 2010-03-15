
/*
* Copyright
*/

#pragma once
#ifndef _FILE_H_
#define _FILE_H_

#include "config.h"
#ifdef _WIN32
#include <windows.h>
#endif
#include "uuid.h"

#ifdef __cplusplus
extern "C" {
#endif

#if FILE_SYS == 1
typedef FILE * udumpfile_handle_t;
#elif FILE_SYS == 2
typedef int udumpfile_handle_t;
#elif FILE_SYS == 3
typedef HANDLE udumpfile_handle_t;
#endif

/// head struct for file
typedef struct {
	udumpfile_handle_t file;
	xc_uuid_t uuid;
	size_t readoff;
	int lock;
} udumpfile_t;

int udumpfile_open(udumpfile_t* file, unsigned long id, const char* filename, int flags);
void udumpfile_close(udumpfile_t* file);

int udumpfile_lock(udumpfile_t* file, int forwrite);
int udumpfile_unlock(udumpfile_t* file);

int udumpfile_append(udumpfile_t* file, const void* ptr, size_t size);
int udumpfile_read(udumpfile_t* file, void* ptr, size_t size);

#ifdef __cplusplus 
} // extern "C"
#endif

#endif // _FILE_H_
