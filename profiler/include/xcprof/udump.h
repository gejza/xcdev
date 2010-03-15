
/*
* Copyright
*/

#pragma once
#ifndef _PROFILER_STREAM_H_
#define _PROFILER_STREAM_H_

#include <xc/symboltable.h>

#ifdef __cplusplus
extern "C" {
#endif

#define WRITE_SRC_INFO 1
#if WRITE_SRC_INFO
#define SRC_DECL , const char* srcfile, int srcline
#define SRC_CC , srcfile, srcline
#define SRC_INFO , __FILE__, __LINE__
#else
#define SRC_DECL 
#define SRC_CC
#define SRC_INFO
#endif

typedef short udump_record_type_t;

typedef struct _udump_t
{
	symboltable_t symbols;
	udumpfile_t out;
} udump_t;

typedef struct _udump_buffer_t
{
	void* ptr;
	size_t size;
	size_t used;
} udump_buffer_t;

typedef struct _udump_entry_t
{
	udump_t* dump;
	udump_buffer_t buffer;
	struct _udump_entry_head_t* head;
} udump_entry_t;

void udump_open(udump_t * dump);
void udump_close(udump_t * dump);

void udump_entry_open(udump_t* dump, udump_entry_t* entry, const char* entryname);
void* udump_entry_record(udump_entry_t* entry, udump_record_type_t type, 
						   const char* name, const void* ptr, size_t size SRC_DECL);
void udump_entry_flush(udump_t* dump, udump_entry_t* entry);
void udump_entry_free(udump_entry_t* entry);

void* udump_buffer_append(udump_buffer_t* buff, const void* ptr, size_t size);

#ifdef __cplusplus 
} // extern "C"
#endif

#endif // _PROFILER_STREAM_H_
