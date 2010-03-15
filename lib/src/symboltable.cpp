
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "xc/symboltable.h"
#include "xc/error.h"

#if 0
const unsigned long IDFILE_SYMBOLTABLE = 'S' | 'T' << 8 | 'D' << 16 | 'B' << 24;


/// structure of write data
#pragma pack(push, 1)
typedef struct _symboltable_entry_t
{
	symid_t id;
	size_t symbolsize;
} symboltable_entry_t;
#pragma pack(pop)

typedef struct _symbolinfo_t
{
	symid_t id;
	char symbol[512];
} symbolinfo_t;

/********* Private functions ************/

///////////
// cache
static symid_t cache_find(symboltable_t* table, const char* symbol)
{
	xc_tree_key_t key = { symbol, strlen(symbol) };
	xc_tree_data_t data;
	if (!xc_tree_search(&table->cache, &key, &data))
		return 0;
	//todo assert
	return *((symid_t*)data.ptr);
}

static int cache_insert(symboltable_t* table, symid_t id, const char* symbol)
{
	{
		xc_tree_key_t key = { symbol, strlen(symbol) };
		xc_tree_data_t data = { &id, sizeof(symid_t) };
		if (!xc_tree_insert(&table->cache, &key, &data))
			return XCERR_FAILED;
	}
	if (table->flags & SYMTAB_REVERSE)
	{
		const xc_tree_key_t key = { &id, sizeof(symid_t) };
		const xc_tree_data_t data = { (char*)symbol, strlen(symbol) };
		if (!xc_tree_insert(&table->cache, &key, &data))
			return XCERR_FAILED;
	}
	return XC_OK;
}

///////////
// file
static int read_next_symbol(udumpfile_t* file, symbolinfo_t* sym)
{
	symboltable_entry_t s;
	int status;

	assert(sym);
	status = udumpfile_read(file, &s, sizeof(s));
	if (status)
		return status;

	assert(s.symbolsize < sizeof(sym->symbol));//todo
	status = udumpfile_read(file, sym->symbol, s.symbolsize);
	if (status)
		return status;

	sym->id = s.id;
	sym->symbol[s.symbolsize] = 0;

	return XC_OK;
}

static symid_t table_load(symboltable_t* table, const char* tosymbol)
{
	int status;
	symid_t id = 0;
	symbolinfo_t sym;

	while ((status = read_next_symbol(&table->file, &sym)) == XC_OK)
	{
		cache_insert(table, sym.id, sym.symbol);

		if (table->lastid < sym.id)
			table->lastid = sym.id; //todo
		if (tosymbol && strcmp(sym.symbol, tosymbol) == 0)
		{
			id = sym.id;
			if (!(table->flags & SYMTAB_FULLLOAD))
				break;
		}
	}
	return id;
}

static int create_symbol(symboltable_t* table, symid_t id, const char* symbol)
{
	symboltable_entry_t s;
	int status;

	s.id = id;
	s.symbolsize = strlen(symbol);
	status = udumpfile_append(&table->file, &s, sizeof(s));
	if (status)
		return status;

	status = udumpfile_append(&table->file, symbol, s.symbolsize);
	if (status)
		return status;

	cache_insert(table, id, symbol);
	table->file.readoff += sizeof(s) + s.symbolsize;

	return XC_OK;
}

/********* Public functions ************/
/**
 * Open file with symbol table and init structure. 
 */
int symboltable_init(symboltable_t* table, const char* filename, unsigned int flags)
{
	int res;
	memset(table, 0, sizeof(symboltable_t));
	xc_pool_init(&table->pool, 1000); // pro cache
	table->flags = filename ? flags : flags|SYMTAB_MEMORY;
	table->filename = filename ? xc_pool_strdup(&table->pool, filename):NULL;
	if (filename)
	{
		res = udumpfile_open(&table->file, IDFILE_SYMBOLTABLE, filename, 0);
		if (res) return res;
		table->uuid = &table->file.uuid;
	}
	else
	{
		table->uuid = (xc_uuid_t*)xc_pool_alloc(&table->pool, sizeof(xc_uuid_t));
		xc_uuid_generate(table->uuid);
	}
	if (!xc_tree_init(&table->cache, &table->pool))
		return XCERR_FAILED;

	table->lock = system_mutex_create();

	if (table->flags & SYMTAB_FULLLOAD)
	{
		table_load(table, NULL);
	}

	return XC_OK;
}

/**
 * Close symbol table
 */
int symboltable_close(symboltable_t* table)
{
	if (!(table->flags & SYMTAB_MEMORY))
		udumpfile_close(&table->file);

	if (!xc_tree_destroy(&table->cache))
		return XCERR_FAILED;
	xc_pool_destroy(&table->pool);
	system_mutex_delete(table->lock);
	return XC_OK;
}

/**
 * Get symbol id from string. If symbol not exists, create it.
 */
symid_t symboltable_get(symboltable_t* table, const char* symbol)
{
	symid_t id;
	int status;

	if (!symbol || !*symbol)
		return 0;

	id = cache_find(table, symbol);
	if (id)
		return id;

	if (table->flags & SYMTAB_MEMORY)
	{
		id = ++table->lastid;
		cache_insert(table, id, symbol);
		return id;
	}
	// generate new

	// get file lock
	udumpfile_lock(&table->file, 0);
	// read symbols
	id = table_load(table, symbol);
	// write new record
	if (!id)
	{
		id = ++table->lastid;
		create_symbol(table, id, symbol);
	}

	// unlock file
	udumpfile_unlock(&table->file);
	return id;
}

symid_t symboltable_get_ts(symboltable_t* table, const char* symbol)
{
	symid_t id;
	system_lock(table->lock);
	id = symboltable_get(table, symbol);
	system_unlock(table->lock);
	return id;
}

/**
 * Translate symbol id to null terminated string. 
 */
size_t symboltable_translate(symboltable_t* table, 
							 const symid_t id, 
							 char* str, size_t size)
{
	// find in cache
	xc_tree_key_t key = { &id, sizeof(symid_t) };
	xc_tree_data_t data;
	if (!xc_tree_search(&table->cache, &key, &data))
		return 0;
	if (data.size+1 > size)
		return data.size+1;
	memcpy(str, data.ptr, data.size);
	str[data.size] = 0;
	return data.size+1;
}

/**
 * Get symbol table statistics
 */
void symboltable_stat(symboltable_t* table, symboltable_stat_t* stat)
{
	memset(stat, 0, sizeof(symboltable_stat_t));
	stat->filename = table->filename ? table->filename:"memory";
	xc_uuid_copy(&stat->uuid, table->uuid);
	stat->count = table->lastid;
	stat->pool_num = table->pool.count;
	stat->pool_free = table->pool.mem_free;
	stat->pool_used = table->pool.mem_used;
	stat->pool_alloc = table->pool.mem_alloc;
}

/**
 * Symbol table dump
 */
int symboltable_dump(const char* fn, symboltable_dumpfnc_t fnc, void* arg)
{
	int status;
	udumpfile_t f;
	symbolinfo_t sym;

	status = udumpfile_open(&f, IDFILE_SYMBOLTABLE, fn, 0);
	if (status) return status;

	while ((status = read_next_symbol(&f, &sym)) == XC_OK)
	{
		fnc(sym.id, sym.symbol, arg);
	}
	/*while (1)
	{	
		symboltable_entry_t s;

		status = udumpfile_read(&table->file, table->syncsize, &s, sizeof(s));
		if (status)
			return status;

		assert(s.symbolsize < sizeof(sym->symbol));//todo
		status = udumpfile_read(&f, table->syncsize + sizeof(s), 
							sym->symbol, s.symbolsize);
	if (status)
		return status;*/


	/*todo symboltable_entry_t entry;
	symboltable_file_t file;
	int status;
	file.filename = fn;
	
	status = file_open(&file, SYMTAB_READONLY);
	if (status != XC_OK)
		return status;
	// read header
	
	
	while (fread(&entry, sizeof(entry), 1, file.file) == 1)
	{
		char buff[512];
		assert(entry.symbolsize < 500);
		fread(buff, 1, entry.symbolsize, file.file);
		buff[entry.symbolsize] = 0;
	}
	
	status = file_close(&file);
	if (status != XC_OK)
		return status;*/

	return XC_OK;
}

#endif

