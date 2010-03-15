
#include "StdAfx.h"
#include "dbm.h"
#include "hash.h"
#include "dbutil.h"

namespace xcdb {

IndexHash::IndexHash(Dbm& db, Pager& pager, const IndexParam& param)
 : Index(db, pager, param), m_first(0)
{
	if (!m_param.hashtable)
		m_param.hashtable = 130;
	size_t s = sizeof(IndexParam) + sizeof(RecordRef) * m_param.hashtable;
	m_main.id = pager.Create(s, &m_main.data);

	unsigned char* ptr = m_main.data.ptr;
	memcpy(ptr, &m_param, sizeof(IndexParam));
	ptr += sizeof(IndexParam);
	// dalsi parametry
	memset(ptr, 0, sizeof(RecordRef) * m_param.hashtable);
}

/* This is based on the hash agorithm from gdbm */
HashNum IndexHash::Hash(const void* data, size_t len)
{
	HashNum value;	/* Used to compute the hash value.  */
	size_t i;	/* Used to cycle through random values. */

	/* Set the initial value from the key size. */
	for (value = 0x238F13AF * HashNum(len), i=0; i < len; i++)
		value = (value + (reinterpret_cast<const char*>(data)[i]
		                    << (i*5 % 24)));

	return (1103515243 * value + 12345);  
}

void IndexHash::Add(RecordRef& rec, const void* key, size_t len)
{
	HashNum n = Hash(key, len);
	// najit odkaz
	RecordRef* ref = byte_cast<RecordRef*>
		                (m_main.data.ptr, sizeof(IndexParam))
						    + (n % m_param.hashtable);
	PageInfo page = {0};
	PageId pageId = 0;
	while (ref->page)
	{
		if (pageId != ref->page)
		{
			pageId = ref->page;
			m_pager.Get(ref->page, &page);
		}
		ref = &(byte_cast<HashItem*>(page.ptr,ref->off)->next);
	}

	// vytvorit zaznam
	const Size recsize = sizeof(HashItem) + len;
	RecordPtr recptr;
	const size_t pagesize = 1024;

	// zkusit volny stranky
	if (m_first) 
	{
		pageId = m_first;
		while (pageId)
		{
			PageManager m(m_pager, pageId);
			if (m.Alloc(recsize, &recptr))
				break;
			if (!m.State().next_page)
			{
				PageId id;
				PageManager n(m_pager, &id, pagesize);
				n.State().prev_page = pageId;
				m.State().next_page = pageId = id;
				n.Alloc(recsize, &recptr); //todo throw
				break;
			}
			else
				pageId = m.State().next_page;
		}
	}
	else
	{
		PageManager(m_pager, &m_first, pagesize).Alloc(recsize, &recptr); //todo throw
		pageId = m_first;
	}

	// ulozit item
	ref->page = pageId;
	ref->off = recptr.off;
	HashItem* item = reinterpret_cast<HashItem*>(recptr.ptr);
	memset(item, 0, sizeof(HashItem));
	item->hash = n;
	item->rec = rec;
	item->keysize = short(recptr.size - sizeof(HashItem));
	item->keylen = short(len);
	memcpy(item+1, key, len);
}

const HashItem* IndexHash::Find(const HashItem* item, HashNum hash) const
{
	const RecordRef* ref = item 
		? &item->next
		: byte_cast<const RecordRef*>
		                (m_main.data.ptr, sizeof(IndexParam))
						    + (hash % m_param.hashtable);
	while (ref->page)
	{
		PageManager page(m_pager, ref->page);
		item = reinterpret_cast<const HashItem*>(page.Case(ref->off));
		if (item->hash == hash)
			return item;
		ref = &item->next;
	}
	return false;
}

Cursor* IndexHash::Select(const void* key, size_t len)
{
	CursorHash* c = new CursorHash(m_db, *this);
	c->Select(key, len);
	return c;
}

/////////////////////////////////////////////////////
// Cursor
CursorHash::CursorHash(const Dbm& db, const IndexHash& index)
 : Cursor(db), m_index(index), m_key(NULL), m_item(NULL)
{
}

void CursorHash::Select(const void* key, size_t len)
{
	m_key = key;
	m_len = len;
	m_hash = m_index.Hash(key, len);
}

bool CursorHash::Next()
{
	if (!m_key)
		return false;
	while (1)
	{
		m_item = m_index.Find(m_item, m_hash);
		if (!m_item)
		{
			m_key = NULL;
			return false;
		}
		// check
		if (m_item->keylen != m_len)
			continue;
		if (memcmp(m_key, m_item + 1, m_len))
			continue;
		SetData(m_item->rec);
		return true;
	}
}

} // namespace xcdb
