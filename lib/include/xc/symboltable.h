
/*
* Copyright
*/

#pragma once
#ifndef _SYMBOL_TABLE_H_
#define _SYMBOL_TABLE_H_

#include "config.h"
#include "pool.h"
#include "tree.h"
#include "system.h"
#include "file.h"
#include "uuid.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Symbol table */

typedef short symid_t; ///< Type of symbol id
typedef void (*symboltable_dumpfnc_t)(symid_t id, const char* symbol, void* arg);

/// Flag options
enum
{
	SYMTAB_READONLY = 0x01,
	SYMTAB_NOCREATE = 0x04,
	SYMTAB_FULLLOAD = 0x08,
	SYMTAB_NOHOLDFS = 0x20,
	SYMTAB_REVERSE = 0x40,
	SYMTAB_MEMORY =0x80,
};

/** Structure to store table status and symbol cache */
typedef struct _symboltable_t
{
	xc_pool_t pool; ///< memory pool for cache
	xc_tree_t cache; ///< binary tree for cache
	unsigned int flags; ///< options
	udumpfile_t file; ///< file struct
	const char* filename; ///< file's name
	symid_t lastid; ///< last created id
	mutex_t lock; ///< mutex for lock
	xc_uuid_t* uuid; ///< unique id
} symboltable_t;

/** Statistics info */
typedef struct _symboltable_stat_t
{
	const char* filename;
	xc_uuid_t uuid;
	int count; ///< Count of stored symbols
	int    pool_num; ///< Count of pool blocks
	size_t pool_used; ///< Used memory in pool
	size_t pool_free; ///< Free memory in pool
	size_t pool_alloc; ///< Allocated memory
} symboltable_stat_t;

/**
 * Open file with symbol table and init structure. 
 * @param table Output handle
 * @param filename File name to store symbol table, 
                   if not exists, create it. 
				   If is null, symbol table stored only in memory
 * @param flags Specify options
 * @return Status opening table
 * @retval STS_OK Table is succesfully open
 */
XC_API int symboltable_init(symboltable_t* table, const char* filename, unsigned int flags);

/**
 * Close symbol table
 * @param table Handle to table
 * @return Status code of closing table
 */
XC_API int symboltable_close(symboltable_t* table);

/**
 * Get symbol id from string. If symbol not exists, create it.
 * @param table Handle to table
 * @param symbol Symbol name
 * @return Id of symbol
 * @retval 0 Unknown error
 */
XC_API symid_t symboltable_get(symboltable_t* table, const char* symbol);

/**
 * Translate symbol id to null terminated string. 
 * @param table Handle to table
 * @param id Id of symbol
 * @param str Buffer to output
 * @param size Size of output buffer
 * @return Number od bytes copied to buffer (including 0 end).
 * @retval > size. Buffer too small. Need buffer of minimal size as ret value
 * @retval 0 Symbol not found
 */
XC_API size_t symboltable_translate(symboltable_t* table, 
							 const symid_t id, 
							 char* str, size_t size);

/**
 * Get symbol table statistics
 * @param table Handle to table
 * @param stat Output structure statistics.
 */
XC_API void symboltable_stat(symboltable_t* table, symboltable_stat_t* stat);

/**
 * Symbol table dump
 * @param file File name to dump.
 * @param fnc Function to run for all symbols
 * @param arg Place argument to function
 */
XC_API int symboltable_dump(const char* file, symboltable_dumpfnc_t fnc, void* arg);

#ifdef __cplusplus 
} // extern "C"
#endif

#endif // _SYMBOL_TABLE_H_

/*
* Copyright
*/

#pragma once
#ifndef _SYMBOL_TABLE_HPP_
#define _SYMBOL_TABLE_HPP_

#include "symboltable.h"
#include "string.hpp"

namespace xc {

class SymbolTable
{
	symboltable_t table;
public:
	SymbolTable(const char* filename, unsigned int flags = 0)
	{
		symboltable_init(&table, filename, flags);
	}
	~SymbolTable()
	{
		symboltable_close(&table);
	}
	operator symboltable_t* ()
	{
		return &table;
	}
	symid_t Get(const char* symbol)
	{
		return symboltable_get(&table, symbol);		
	}
	const String Translate(symid_t id)
	{
		String s;
		size_t sz = 32;
		while (1)
		{
			/*size_t sz = symboltable_translate(&table, id, s.Ptr(sz), sz);
			if (!sz) return "";
			if (sz <= s.Size())
			{
				s.Resize(sz-1); break;
			}*/
		}
		return s;
	}
	void stat(symboltable_stat_t* stat)
	{
		symboltable_stat(&table, stat);
	}
};

} // namespace xc

#endif // _SYMBOL_TABLE_HPP_
