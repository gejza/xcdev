
#include <stdio.h>
#include <assert.h>
#include "xcprof/udump.h"
#include "xc/symboltable.h"

/********* Private structures ************/
#pragma pack(push, 1)
typedef struct _udump_entry_head_t
{
	symid_t name;
	size_t size;
} udump_entry_head_t;

typedef struct _udump_record_t
{
	udump_record_type_t type;
	symid_t name;
#if WRITE_SRC_INFO
	symid_t srcfile;
	int srcline;
#endif
} udump_record_t;
#pragma pack(pop)


/********* Private functions ************/
__inline size_t udump_buffer_getfreesize(udump_buffer_t* buff)
{
	return buff->size - buff->used;
}

void* udump_buffer_append(udump_buffer_t* buff, const void* ptr, size_t size)
{
	void * p;
	if (!buff->ptr || udump_buffer_getfreesize(buff) < size)
	{
		buff->size += size * 4;
		buff->ptr = realloc(buff->ptr, buff->size);
		assert(buff->ptr);
		if (!buff->ptr)
			return NULL;
	}
	p = ((unsigned char*)buff->ptr) + buff->used;
	if (ptr)
		memcpy(p, ptr, size);
	buff->used += size;
	return p;
}

/********* Public functions ************/
void udump_open(udump_t * dump)
{
	symboltable_init(&dump->symbols, "profiler.sym", 0);
	udumpfile_open(&dump->out, 1, "profiler.dump", 0);
}

void udump_close(udump_t * dump)
{
	// smazat buffer?
	udumpfile_close(&dump->out);
	symboltable_close(&dump->symbols);
}

void udump_entry_open(udump_t* dump, udump_entry_t* entry, const char* entryname)
{
	entry->dump = dump;
	entry->head = udump_buffer_append(&entry->buffer, NULL, sizeof(udump_entry_head_t));
	entry->head->name = symboltable_get(&dump->symbols, entryname);
}

void* udump_entry_record(udump_entry_t* entry, udump_record_type_t type, 
				  const char* name, const void* ptr, size_t size SRC_DECL)
{
	udump_record_t rec;
	rec.type = type;
	rec.name = symboltable_get(&entry->dump->symbols, name);
#if WRITE_SRC_INFO
	rec.srcfile = symboltable_get(&entry->dump->symbols, srcfile);
	rec.srcline = srcline;
#endif
	// pouze pridat do bufferu
	udump_buffer_append(&entry->buffer, &rec, sizeof(rec));
	return udump_buffer_append(&entry->buffer, ptr, size);
}

void udump_entry_flush(udump_t* dump, udump_entry_t* entry)
{
	entry->head->size = entry->buffer.used;
	udumpfile_append(&dump->out, entry->buffer.ptr, entry->buffer.used);
	//printf("Write %d bytes to dump.\n", entry->buffer.used);
	entry->buffer.used = 0;
	entry->dump = 0;
}

void udump_entry_free(udump_entry_t* entry)
{
	if (entry->buffer.ptr)
		free(entry->buffer.ptr);
	entry->buffer.ptr = NULL;
	entry->buffer.size = 0;
}




