
#include <stdio.h>
#ifndef _WIN32
#include <sys/io.h>
#endif
#include <fcntl.h>
#include <string.h>
#include "xc/file.h"
#include "xc/error.h"
#include "xc/symboltable.h"

#if 0
typedef struct {
	unsigned long idfile;
	xc_uuid_t uuid;
	int head_size;
	int symid_size;
	int size_t_size;
} udumpfile_header_t;

static udumpfile_handle_t sys_open(const char* filename);
static void sys_close(udumpfile_handle_t file);
static int sys_write(udumpfile_handle_t file, const void* ptr, size_t size);
static int sys_read(udumpfile_handle_t file, void* ptr, size_t size);
static void sys_flush(udumpfile_handle_t file);
static void sys_seek(udumpfile_handle_t file, size_t off);
static void sys_seekeof(udumpfile_handle_t file);
static void sys_lock(udumpfile_handle_t file);
static void sys_unlock(udumpfile_handle_t file);

/////////////////////////
int udumpfile_open(udumpfile_t* file, unsigned long id, const char* filename, int flags)
{
	udumpfile_header_t head;

	memset(file, 0, sizeof(udumpfile_t));
	file->file = sys_open(filename);
	if (!file->file)
	{
		return xc_error_fatal(XCERR_NOTOPEN, "Failed open file %s", filename);
	}
	// read header
	sys_seek(file->file, 0);
	if (sys_read(file->file, &head, sizeof(head)))
	{
		file->uuid = head.uuid;
	}
	else
	{
		head.idfile = id;
		head.head_size = sizeof(udumpfile_header_t);
		xc_uuid_generate(&head.uuid);
		head.symid_size = sizeof(symid_t);
		head.size_t_size = sizeof(size_t);
		sys_write(file->file, &head, sizeof(head));
	}
	file->readoff = head.head_size;

	return 0;
}

void udumpfile_close(udumpfile_t* file)
{
	sys_close(file->file);
}
#if 0
int udumpfile_lock(udumpfile_t* file, int forwrite)
{
	if (!file->lock)
		sys_lock(&file->file);
	if (forwrite)
		file->lock |= 0x10000;
	file->lock++;
	return 0;
}
int udumpfile_unlock(udumpfile_t* file)
{
	file->lock--;
	if (!(file->lock & 0xffff))
	{
		if (file->lock & 0x10000)
			sys_flush(file->file);
		sys_unlock(&file->file);
		file->lock = 0;
	}
	return 0;
}
#endif

int udumpfile_append(udumpfile_t* file, const void* ptr, size_t size)
{
	udumpfile_lock(file, 1);
	sys_seekeof(file->file);
	sys_write(file->file, ptr, size);
	udumpfile_unlock(file);
	return 0;
}

int udumpfile_read(udumpfile_t* file, void* ptr, size_t size)
{
	sys_seek(file->file, file->readoff);
	if (sys_read(file->file, ptr, size)) 
	{
		file->readoff += size;
		return 0;
	}
	return XCERR_EOF;
}

// FILE
#if FILE_SYS == 1

static udumpfile_handle_t sys_open(const char* filename)
{
	return fopen(filename, "a+");
}

static void sys_close(udumpfile_handle_t file)
{
	fclose(file);
}

static int sys_write(udumpfile_handle_t file, const void* ptr, size_t size)
{
	return fwrite(ptr, size, 1, file) == 1;
}

static int sys_read(udumpfile_handle_t file, void* ptr, size_t size)
{
	return fread(ptr, size, 1, file) == 1;
}

static void sys_flush(udumpfile_handle_t file)
{
	fflush(file);
}

static void sys_seek(udumpfile_handle_t file, size_t off)
{
	fseek(file, (long)off, SEEK_SET);
}

static void sys_seekeof(udumpfile_handle_t file)
{
	fseek(file, 0, SEEK_END);
}

static void sys_lock(udumpfile_handle_t file)
{
}

static void sys_unlock(udumpfile_handle_t file)
{
}

#endif

#if FILE_SYS == 2

static udumpfile_handle_t sys_open(const char* filename)
{
	return _open(filename, _O_APPEND|_O_BINARY|_O_SEQUENTIAL|_O_RDWR|_O_CREAT);
}

static void sys_close(udumpfile_handle_t file)
{
	_close(file);
}

static int sys_write(udumpfile_handle_t file, const void* ptr, size_t size)
{
	return _write(file, ptr, size) == size;
}

static int sys_read(udumpfile_handle_t file, void* ptr, size_t size)
{
	return _read(file, ptr, size) == size;
}

static void sys_flush(udumpfile_handle_t file)
{
	_commit(file);
}

static void sys_seek(udumpfile_handle_t file, size_t off)
{
	_lseek(file, (long)off, SEEK_SET);
}

static void sys_seekeof(udumpfile_handle_t file)
{
	_lseek(file, 0L, SEEK_END);
}

#endif

#if FILE_SYS == 3

static udumpfile_handle_t sys_open(const char* filename)
{
	return CreateFile(filename, GENERIC_WRITE|GENERIC_READ,
		FILE_SHARE_WRITE|FILE_SHARE_READ, NULL, OPEN_ALWAYS, 0, NULL);
}

static void sys_close(udumpfile_handle_t file)
{
	CloseHandle(file);
}

static int sys_write(udumpfile_handle_t file, const void* ptr, size_t size)
{
	DWORD written;
	return WriteFile(file, ptr, (DWORD)size, &written, NULL) && written == size;
}

static int sys_read(udumpfile_handle_t file, void* ptr, size_t size)
{
	DWORD read;
	return ReadFile(file, ptr, (DWORD)size, &read, NULL) && read == size;
}

static void sys_flush(udumpfile_handle_t file)
{
	//fflush(file);
}

static void sys_seek(udumpfile_handle_t file, size_t off)
{
	SetFilePointer(file, off, NULL, FILE_BEGIN);
}

static void sys_seekeof(udumpfile_handle_t file)
{
	SetFilePointer(file, 0, NULL, FILE_END);
}

static void sys_lock(udumpfile_handle_t file)
{
	BOOL l = LockFile(file, 0, 0, ~0, 0);
	l;
}

static void sys_unlock(udumpfile_handle_t file)
{
	UnlockFile(file, 0, 0, ~0, 0);
}

#endif
#endif
