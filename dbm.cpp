#include "StdAfx.h"
#include "dbm.h"
#include "hash.h"
#include "tree.h"

namespace xcdb {

struct Record
{
	size_t size;
	size_t len;
};

///
struct IndexData
{
	PageId page;
	IndexParam param;
	char name[32];
};

//////////////////////////////////////////
Dbm::Dbm(Pager& pager) : m_pager(pager)
{
}

Dbm::~Dbm(void)
{
}

void Dbm::AddToIndex(RecordRef& rec, const Key& key)
{
	AddToIndex(rec, key.name, key.key, key.keylen);
}

void Dbm::AddToIndex(RecordRef& rec, const char* name, const void* key, size_t len)
{
	Index * i = GetIndex(name);
	assert(i);
	i->Add(rec, key, len);
}

void Dbm::AddToIndex(RecordRef& rec, const char* name, const char* key)
{
	AddToIndex(rec, name, key, strlen(key));
}

void Dbm::Insert(const void* data, size_t len, RecordRef* ref, const Key** keys)
{
	// insert
	RecordRef rec;
	PageInfo page;
	rec.page = m_pager.Create(len, &page);
	rec.off = 0;
	memcpy(page.ptr, data, len);

	// index
	for (;keys && *keys;keys++)
	{
		AddToIndex(rec, **keys);
	}
	if (ref)
		*ref = rec;
}

void Dbm::Lock(const RecordRef& rec, LockInfo* lock) const
{
	PageInfo page;
	m_pager.Get(rec.page, &page);
	lock->ptr = page.ptr;
	lock->len = page.size;
}

Index* Dbm::CreateIndex(const char* name, const IndexParam& param)
{
	Index* i;
	switch (param.type)
	{
	case IndexParam::Hash:
		i = new IndexHash(*this, m_pager, param);
		break;
	case IndexParam::BTree:
		i = new IndexTree(*this, m_pager, param);
		break;
	default:
		throw; //todo
	};
	// vyhodit chybu pokud existuje
	m_indexes.insert(std::make_pair(name, i));
	return i;
}

Index* Dbm::GetIndex(const char* name)
{
	IndexMap::iterator i = m_indexes.find(name);
	return i != m_indexes.end() ? i->second:NULL;
}

const Index* Dbm::GetIndex(const char* name) const
{
	IndexMap::const_iterator i = m_indexes.find(name);
	return i != m_indexes.end() ? i->second:NULL;
}

//////////////////////////////////////////////
// Index
Index::Index(Dbm& db, Pager& pager, const IndexParam& param)
 : m_db(db), m_pager(pager)
{
	m_param = param;
}

Cursor* Index::Select(const char *key)
{
	return Select(key, strlen(key));
}

///////////////////////////////////////////////////
Cursor::Cursor(const Dbm& db) : m_db(db)
{
}

void Cursor::SetData(const RecordRef& rec)
{
	m_db.Lock(rec, &m_lock);
}

size_t Cursor::GetRecSize()
{
	/*if (m_lock.len)
		return m_lock.len;
	m_db.Lock(m_item->rec, &m_lock);*/
	return m_lock.len;
}

const void* Cursor::Fetch()
{
	GetRecSize();
	return m_lock.ptr;
}

size_t Cursor::Fetch(void* buff, size_t size)
{
	memcpy(buff, Fetch(), GetRecSize());
	return GetRecSize();
}

} // namespace xcdb


