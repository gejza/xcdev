
/*
* Copyright
*/

#pragma once
#ifndef _TREE_H_
#define _TREE_H_

#include "pool.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _xc_tree_t 
{
	xc_pool_t *pool;
	struct _xc_tree_node_t *root;
} xc_tree_t;

typedef struct _xc_tree_key_t
{
	const void* ptr;
	size_t size;
} xc_tree_key_t;

typedef struct _xc_tree_data_t
{
	void* ptr;
	size_t size;
} xc_tree_data_t;


/**
 * Init tree
 * @param tree Output handle to tree
 * @param pool Pool to used. If NULL use owned pool
 * @return Positive value on success
 */
XC_API int xc_tree_init(xc_tree_t *tree, xc_pool_t* pool);

/**
 * Destroy tree
 * @param tree Handle to tree
 * @return Positive value on success
 */
XC_API int xc_tree_destroy(xc_tree_t *tree);

/**
 * Create key in tree
 * @param tree Handle to tree
 * @param key Key to insert
 * @param datasize Size of data to create
 * @return Pointer to data
 */
XC_API void* xc_tree_new(xc_tree_t *tree, 
				const xc_tree_key_t* key,
				size_t datasize);

/**
 * Insert to tree
 * @param tree Handle to tree
 * @param key Key to insert
 * @param data Data to insert
 * @return Positive value on success
 */
XC_API int xc_tree_insert(xc_tree_t *tree, 
				const xc_tree_key_t* key,
				const xc_tree_data_t* data);

/**
 * Remove from tree
 * @param tree Handle to tree
 * @param key Key to insert
 * @return Positive value on success
 */
XC_API int xc_tree_remove(xc_tree_t *tree, const xc_tree_key_t* key);

/**
 * Find key in tree
 * @param key Key to find
 * @param data Data to output
 * @return Positive value on success
 */
XC_API int xc_tree_search(const xc_tree_t *tree,
			  const xc_tree_key_t* key,
			  xc_tree_data_t* data);

/**
 * Check if tree is red-black
 * @param tree Handle to tree
 * @return Number of levels
 */
XC_API int xc_tree_check(const xc_tree_t *tree);

#ifdef __cplusplus 
} // extern "C"
#endif

#endif // _TREE_H_

/*
* Copyright
*/

#pragma once
#ifndef _XC_TREE_HPP_
#define _XC_TREE_HPP_

#include "pool.hpp"
#include "tree.h"

namespace xc {

class Tree
{
	xc_tree_t tree;
	Pool pool;
public:
	class Key
	{
	};

	class Data
	{
	};


	Tree()
	{
		xc_tree_init(&tree, pool);
	}
	~Tree()
	{
		xc_tree_destroy(&tree);
	}
	void Insert(const char* k, int d)
	{
		xc_tree_key_t key = { k, strlen(k) };
		xc_tree_data_t data = { &d, sizeof(d) };
		xc_tree_insert(&tree, &key, &data);
	}
	int Search(const char* k)
	{
		xc_tree_key_t key = { k, strlen(k) };
		xc_tree_data_t data;
		if (!xc_tree_search(&tree, &key, &data))
			return 0;
		return *((int*)data.ptr);
	}
	void Remove(int k)
	{
		xc_tree_key_t key = { &k, sizeof(k) };
		xc_tree_remove(&tree, &key);
	}
	void Stat()
	{
		pool.Stat();
	}
};

} // namespace xc

#endif // _XC_TREE_HPP_
