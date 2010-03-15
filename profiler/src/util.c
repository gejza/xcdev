
#include <stdio.h>
#include <assert.h>
#include <memory.h>
#include "xcprof/util.h"
#include "xc/tree.h"

/********* Thread safe functions ************/
#if PROFILER_TS

void threadsafe_inc(threadsafe_t* ts)
{
	if (!ts->ref)
	{
		pool_init(&ts->pool, 1000);
		tree_init(&ts->child, &ts->pool);
		ts->mutex = system_mutex_create();
	}
	ts->ref++;
}

void threadsafe_dec(threadsafe_t* ts)
{
	ts->ref--;
	if (!ts->ref)
	{
		tree_destroy(&ts->child);
		pool_destroy(&ts->pool);
	}
}

void* threadsafe_get_by_id(threadsafe_t* ts, threadid_t id, size_t size)
{
	const tree_key_t key = { &id, sizeof(id) };
	tree_data_t data;
	void* p;
	system_lock(ts->mutex);
	if (tree_search(&ts->child, &key, &data))
	{
		system_unlock(ts->mutex);
		assert(data.size == size); //todo
		return data.ptr;
	}
	p = tree_new(&ts->child, &key, size);
	system_unlock(ts->mutex);
	memset(p, 0, size);
	return p;
}

#endif

