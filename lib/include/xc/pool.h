
/*
* Copyright
*/

#pragma once
#ifndef _MEMORY_POOL_H_
#define _MEMORY_POOL_H_

#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Pool */
typedef struct _xc_pool_t
{
	size_t blocksize;
	int count;
	size_t mem_used;
	size_t mem_free;
	size_t mem_alloc;
	struct _xc_pool_block_t* block;
} xc_pool_t;

/** 
 * Create memory pool in own memory
 */
XC_API xc_pool_t* xc_pool_create(size_t blocksize);

/**
 * Init memory pool. 
 * @param pool Pool structure
 * @param blocksize Size of allocating step
 */
XC_API void xc_pool_init(xc_pool_t* pool, size_t blocksize);

/**
 * Destroy memory pool. 
 * @param pool Pool structure
 */
XC_API void xc_pool_destroy(xc_pool_t* pool);

/**
 * Allocate memory from pool
 * @param pool Pool structure
 * @param size Size of memory
 * @return Pointer to memory or NULL on error
 */
XC_API void* xc_pool_alloc(xc_pool_t* pool, size_t size);

/**
 * Copy memory to pool
 * @param pool Pool structure
 * @param src Pointer to memory to be copied
 * @param size Size of memory
 * @return Pointer to memory or NULL on error
 */
XC_API const void* xc_pool_clone(xc_pool_t* pool, const void* src, size_t size);

/**
 * String duplicate in pool
 * @param pool Pool structure
 * @param str Pointer to null terminated string
 * @return Pointer to string or NULL on error
 */
XC_API const char* xc_pool_strdup(xc_pool_t* pool, const char* str);


// todo
XC_API struct _xc_string_t* xc_string_pool(xc_pool_t* pool, const char* ptr, size_t size);

#ifdef __cplusplus 
} // extern "C"
#endif

#endif // _MEMORY_POOL_H_

/*
* Copyright
*/

#pragma once
#ifndef _XC_POOL_HPP_
#define _XC_POOL_HPP_

#include "pool.h"
#include "string.hpp"

namespace xc {

class Pool
{
	xc_pool_t pool;
public:
	Pool()
	{
		xc_pool_init(&pool, 1000000);
	}
	~Pool()
	{
		xc_pool_destroy(&pool);
	}
	operator xc_pool_t* ()
	{
		return &pool;
	}
	void Stat()
	{
		printf("Cnt: %d Used: %d Free: %d Alloc: %d\n", 
			pool.count, pool.mem_used, pool.mem_free, pool.mem_alloc);
	}
};

/*
class PoolString : public ConstString
{
public:
	PoolString(Pool& pool, const char* ptr = NULL, size_t size = 0)
	{
		data = xc_string_pool(pool, ptr, size);
	}
	const PoolString& operator = (const char* str)
	{
		xc_string_set(&data, str, 0);
		return *this;
	}
};*/

} // namespace xc

#endif // _XC_POOL_HPP_
