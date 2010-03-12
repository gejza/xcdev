
#if 0
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "xc/tree.h"

#define USE_HASH 0

typedef unsigned char byte_t;

#if USE_HASH
typedef unsigned int hash_t;
#endif

typedef struct _xc_tree_node_t {
	int red;
#if USE_HASH
	hash_t hash;
#endif
	size_t keylen;
	size_t datalen;
	union {
		struct {
			struct _xc_tree_node_t *left;
			struct _xc_tree_node_t *right;
		};
		struct _xc_tree_node_t *link[0];
	};
} xc_tree_node_t;

/********* Private functions ************/
#if USE_HASH
static hash_t key_hash(const xc_tree_key_t* key)
{
	register hash_t h = 0x1a2b3c4d;
	register const byte_t* p = (const byte_t*)key->ptr;
	register size_t size = key->size;
	while (size--)
	{
		register hash_t g = h & 0xf0000000;
		h = h ^ (g >> 24);
		h = h ^ g;
		h = h << 4;
		h += *p++;
	}
	return h;
}
#endif

__inline int node_is_red(const xc_tree_node_t *node)
{
	return node != NULL && node->red == 1;
} 

__inline void* node_get_key(const xc_tree_node_t *node)
{
	return ((byte_t*)node) + sizeof(xc_tree_node_t);
}

__inline void* node_get_data(const xc_tree_node_t *node)
{
	return ((byte_t*)node) + sizeof(xc_tree_node_t) + node->keylen;
}

#if USE_HASH
__inline int node_cmpkey(const xc_tree_node_t *node, const xc_tree_key_t* key, hash_t hash)
{
	if (node->hash > hash)
		return -1;
	if (node->hash < hash)
		return 1;
#else
__inline int node_cmpkey(const xc_tree_node_t *node, const xc_tree_key_t* key)
{
#endif

	if (node->keylen > key->size)
		return -1;
	if (node->keylen < key->size)
		return 1;
	return memcmp(node_get_key(node), key->ptr, key->size);
}

__inline int node_cmp(const xc_tree_node_t *node1, const xc_tree_node_t *node2)
{
#if USE_HASH
	if (node1->hash > node2->hash)
		return -1;
	if (node1->hash < node2->hash)
		return 1;
#endif
	if (node1->keylen > node2->keylen)
		return -1;
	if (node1->keylen < node2->keylen)
		return 1;
	return memcmp(node_get_key(node1), node_get_key(node2), node1->keylen);
}

static xc_tree_node_t *node_alloc(xc_tree_t *tree, size_t size)
{
	if (tree->pool)
		return (xc_tree_node_t *)xc_pool_alloc(tree->pool, size);
	return (xc_tree_node_t *)malloc(size);
}

static void node_free(xc_tree_t *tree, xc_tree_node_t *node)
{
	if (!tree->pool)
		free(node);
}

static xc_tree_node_t *node_make(xc_tree_t *tree,
							  const xc_tree_key_t* key,
							  size_t datasize)
{
	xc_tree_node_t *rn = node_alloc(tree, sizeof(xc_tree_node_t) + key->size + datasize);
	if (!rn)
		return NULL;
	
	rn->red = 1; /* 1 is red, 0 is black */
#if USE_HASH
	rn->hash = key_hash(key);
#endif
	rn->keylen = key->size;
	rn->datalen = datasize;
	rn->left = NULL;
	rn->right = NULL;
	memcpy(node_get_key(rn), key->ptr, key->size);
	return rn;
}

static xc_tree_node_t *tree_single(xc_tree_node_t *root, int dir)
{
	xc_tree_node_t *save = root->link[!dir];

	root->link[!dir] = save->link[dir];
	save->link[dir] = root;

	root->red = 1;
	save->red = 0;

	return save;
}

static xc_tree_node_t *tree_double(xc_tree_node_t *root, int dir)
{
	root->link[!dir] = tree_single(root->link[!dir], !dir);
	return tree_single( root, dir);
}

static int tree_check_r(const xc_tree_node_t *root)
{
	int lh, rh;

	if ( root == NULL )
		return 1;
	else {
		xc_tree_node_t *ln = root->left;
		xc_tree_node_t *rn = root->right;

		/* Consecutive red links */
		if (node_is_red(root)) {
			if ( node_is_red ( ln ) || node_is_red ( rn ) ) {
				puts ( "Red violation" );
				return 0;
			}
		}

		lh = tree_check_r( ln );
		rh = tree_check_r( rn );

		/* Invalid binary search tree */
		if ((ln && node_cmp(ln, root) <= 0)
			|| (rn && node_cmp(rn, root) >= 0))
		{
			puts ( "Binary tree violation" );
			return 0;
		}

		/* Black height mismatch */
		if ( lh != 0 && rh != 0 && lh != rh ) {
			puts ( "Black violation" );
			return 0;
		}

		/* Only count black links */
		if ( lh != 0 && rh != 0 )
			return node_is_red ( root ) ? lh : lh + 1;
		else
			return 0;
	}
}

/********* Public functions ************/
/**
 * Init tree
 */
int xc_tree_init(xc_tree_t *tree, xc_pool_t* pool)
{
	memset(tree, 0, sizeof(xc_tree_t));
	tree->pool = pool;
	return 1;
}

/**
 * Destroy tree
 */
int xc_tree_destroy(xc_tree_t *tree)
{
	return 1;
}

/**
 * Create key in tree
 */
void* xc_tree_new(xc_tree_t *tree, 
				const xc_tree_key_t* key,
				size_t datasize)
{
	xc_tree_node_t head = {0}; /* False tree root */
	xc_tree_node_t *g, *t;     /* Grandparent & parent */
	xc_tree_node_t *p, *q;     /* Iterator & parent */
	xc_tree_node_t *ni;        /* New item */
	int dir = 0, last;
#if USE_HASH
	hash_t hash = key_hash(key);
#endif

	/* Insert to empty tree */
	if (tree->root == NULL) {
		/* Empty tree case */
		ni = tree->root = node_make(tree, key, datasize);
		/* Make root black */
		tree->root->red = 0;
		if (!tree->root)
			return NULL;

		return node_get_data(ni);
	}

	/* Set up helpers */
	t = &head;
	g = p = NULL;
	q = t->right = tree->root;

	/* Search down the tree */
	while (1) 
	{
		if (!q) 
		{
			/* Insert new node at the bottom */
			ni = p->link[dir] = q = node_make(tree, key, datasize);
			if ( q == NULL )
				return 0;
		}
		else if (node_is_red(q->left) && node_is_red(q->right)) 
		{
			/* Color flip */
			q->red = 1;
			q->left->red = 0;
			q->right->red = 0;
		}

		/* Fix red violation */
		if (node_is_red(q) && node_is_red(p)) 
		{
			int dir2 = t->right == g;

			if ( q == p->link[last] )
				t->link[dir2] = tree_single( g, !last);
			else
				t->link[dir2] = tree_double( g, !last);
		}

		/* Stop if found */
#if USE_HASH
		if (node_cmpkey(q, key, hash) == 0)
#else
		if (node_cmpkey(q, key) == 0)
#endif
			break;

		last = dir;
#if USE_HASH
		dir = node_cmpkey(q, key, hash) > 0 ? 1:0;
#else
		dir = node_cmpkey(q, key) > 0 ? 1:0;
#endif
		/* Update helpers */
		if ( g != NULL )
			t = g;
		g = p, p = q;
		q = q->link[dir];
	}

	/* Update root */
	tree->root = head.right;

	/* Make root black */
	tree->root->red = 0;

	return node_get_data(ni);
}

/**
* Insert to tree
*/
int xc_tree_insert(xc_tree_t *tree, 
				const xc_tree_key_t* key,
				const xc_tree_data_t* data)
{
	void* p = xc_tree_new(tree, key, data->size);
	if (!p)
		return 0;
	memcpy(p, data->ptr, data->size);
	return 1;
}

/**
* Remove from tree
*/
int xc_tree_remove(xc_tree_t *tree, const xc_tree_key_t* key)
{
	xc_tree_node_t head = {0}; /* False tree root */
	xc_tree_node_t *q, *p, *g; /* Helpers */
	xc_tree_node_t *f = NULL;  /* Found item */
	int dir = 1;
#if USE_HASH
	hash_t hash = key_hash(key);
#endif

	if (!tree->root)
		return 1;

	/* Set up helpers */
	q = &head;
	g = p = NULL;
	q->right = tree->root;

	/* Search and push a red down */
	while (q->link[dir]) 
	{
		int last = dir;

		/* Update helpers */
		g = p, p = q;
		q = q->link[dir];
#if USE_HASH
		dir = node_cmpkey(q, key, hash)  > 0 ? 1:0;
		/* Save found node */
		if (node_cmpkey(q, key, hash) == 0)
#else
		dir = node_cmpkey(q, key)  > 0 ? 1:0;
		/* Save found node */
		if (node_cmpkey(q, key) == 0)
#endif
			f = q;

		/* Push the red node down */
		if (!node_is_red(q) && !node_is_red(q->link[dir]))
		{
			if (node_is_red(q->link[!dir]))
				p = p->link[last] = tree_single(q, dir);
			else if (!node_is_red(q->link[!dir]))
			{
				xc_tree_node_t *s = p->link[!last];

				if (s) 
				{
					if (!node_is_red (s->link[!last]) && !node_is_red(s->link[last]))
					{
						/* Color flip */
						p->red = 0;
						s->red = 1;
						q->red = 1;
					}
					else 
					{
						int dir2 = g->right == p;

						if (node_is_red(s->link[last]))
							g->link[dir2] = tree_double(p, last);
						else if (node_is_red(s->link[!last]))
							g->link[dir2] = tree_single(p, last);

						/* Ensure correct coloring */
						q->red = g->link[dir2]->red = 1;
						g->link[dir2]->left->red = 0;
						g->link[dir2]->right->red = 0;
					}
				}
			}
		}
	}

	/* Replace and remove if found */
	if (f) 
	{
		//f->data = q->data;
		exit(5);// todo
		p->link[p->right == q] =
			q->link[q->left == NULL];
		node_free(tree, q);
	}

	/* Update root and make it black */
	tree->root = head.right;
	if (tree->root)
		tree->root->red = 0;

	return 1;
}

/**
 * Find key in tree
 */
int xc_tree_search(const xc_tree_t *tree,
			  const xc_tree_key_t* key,
			  xc_tree_data_t* data)
{
	xc_tree_node_t* node = tree->root;
#if USE_HASH
	hash_t hash = key_hash(key);
#endif
	register int step = 0;
	while (node)
	{
#if USE_HASH
		int c = node_cmpkey(node, key, hash);
#else
		int c = node_cmpkey(node, key);
#endif
		step++;
		if (c < 0)
			node = node->left;
		else if (c > 0)
			node = node->right;
		else
		{
			data->ptr = node_get_data(node);
			data->size = node->datalen;
			return step;
		};
	}
	return 0;
}

/**
 * Check if tree is red-black
 */
int xc_tree_check(const xc_tree_t *root)
{
	return tree_check_r(root->root);
}
#endif

