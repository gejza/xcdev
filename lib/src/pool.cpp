
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "xc/pool.h"
#include "xc/error.h"
#if 0
typedef struct _xc_pool_block_t 
{
	size_t used;
	size_t free;
	struct _xc_pool_block_t* next;
} xc_pool_block_t;

/********* Private functions ************/
static xc_pool_block_t* pool_newblock(xc_pool_t* pool, size_t size)
{
	xc_pool_block_t* b;

	if (size < pool->blocksize)
		size = pool->blocksize;

	b = (xc_pool_block_t*)malloc(sizeof(xc_pool_block_t) + size);
	if (!b)
	{
		xc_error_fatal(XCERR_OUTOFMEM, "Cannor allocate pool block.");
		return NULL;
	}
	memset(b, 0, sizeof(xc_pool_block_t));
	b->free = size;
	b->next = pool->block;
	pool->block = b;
	pool->count++;
	pool->mem_free += size;
	pool->mem_alloc += sizeof(xc_pool_block_t) + size;
	return b;
}

static void* pool_block_alloc(xc_pool_block_t* block, size_t size)
{
	void* ptr;
	if (block->free < size)
		return NULL;
	ptr = ((char*)block) + sizeof(xc_pool_block_t) + block->used;
	block->used += size;
	block->free -= size;
	return ptr;
}

/********* Public functions ************/

/**
 * Init memory pool. 
 */
void xc_pool_init(xc_pool_t* pool, size_t blocksize)
{
	memset(pool, 0, sizeof(xc_pool_t));
	pool->blocksize = blocksize;
}

/**
 * Destroy memory pool. 
 */
void xc_pool_destroy(xc_pool_t* pool)
{
	xc_pool_block_t* block = pool->block;
	while (block)
	{
		xc_pool_block_t* next = block->next;
		free(block);
		block = next;
	}
	memset(pool, 0, sizeof(xc_pool_t));
}

/**
 * Allocate memory from pool
 */

void* xc_pool_alloc(xc_pool_t* pool, size_t size)
{
	void* ptr;
	ptr = pool->block ? pool_block_alloc(pool->block, size):NULL;
	if (!ptr)
		ptr = pool_block_alloc(pool_newblock(pool, size * 100), size);

	pool->mem_used += size;
	pool->mem_free -= size;
	return ptr; 
}

/**
 * Copy memory to pool
 */
const void* xc_pool_clone(xc_pool_t* pool, const void* src, size_t size)
{
	void* p = xc_pool_alloc(pool, size);
	if (!p)
		return NULL;
	memcpy(p, src, size);
	return p;
}

/**
 * String duplicate in pool
 */
const char* xc_pool_strdup(xc_pool_t* pool, const char* str)
{
	return (const char*)xc_pool_clone(pool, str, strlen(str) + 1);
}

///////////////////////////////////////////
#include "xc/string.h"
////////////////////////////////////////////////////////
// default type
typedef struct _xc_string_pool_t
{
	xc_string_type_t* type;
	xc_pool_t* pool;
} xc_string_pool_t;

static xc_string_pool_t* pool_string_realloc(xc_string_pool_t* s, size_t size)
{
	/*size_t a = xc_align(xc_string_type_pool_string.off_ptr + size + 1, 16);
	s = realloc(s, a);
#ifdef _XC_STRING_DEBUG
	printf("string: realloc %ld bytes at %p.\n", a, s);
#endif
	if (!s)
		xc_error_fatal(XCERR_OUTOFMEM, "Out of memory for allocate string.");
	s->size = size;
	s->reserved = a - sizeof(xc_string_pool_t);
	return s;*/
}

static xcbool_t FASTCALL pool_string_onwrite(xc_string_t** data, size_t size, xcbool_t copy)
{
	xc_string_t* prev = *data;
	if (!prev)
	{
		//todo hodi se na assert
		return xcfalse;
	}

	*data = xc_string_pool(((xc_string_pool_t*)prev)->pool, NULL, size);
	if (copy)
	{
		memcpy(xc_string_pointer(*data),
			   xc_string_pointer(prev),
			   xc_min(size, xc_string_size(prev)));
	}
	return xctrue;
}

#define xc_offsetof(s, m) ((size_t)((s*)0)->m)

static xc_string_type_t xc_string_type_pool = 
{
	/* name = */ "string at pool",
	/* flags = */ XC_STRING_END,
	/* off_ptr = */ sizeof(xc_string_pool_t),
	/* _ptr = */ 0, 
	/* _incref = */ 0, 
	/* _decref = */ 0, 
	/* _onwrite = */ pool_string_onwrite,
};

XC_API xc_string_t* xc_string_pool(xc_pool_t* pool, const char* ptr, size_t size)
{
	// initialize string
	xc_string_pool_t* str;
	size_t alloc;
	char* p;

	if (!size && ptr)
		size = xc_strlen(ptr);
	alloc = sizeof(xc_string_pool_t) + size + 1;
	str = xc_pool_alloc(pool, alloc);
	str->type = &xc_string_type_pool;
	str->pool = pool;
	p = xc_string_pointer((xc_string_t*)str);
	if (ptr)
	{
		memcpy(p, ptr, size);
	}
	p[size] = '\0';
	return (xc_string_t*)str;
}
#endif

