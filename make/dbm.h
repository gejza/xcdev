#pragma once

#include "page.h"

namespace xcdb {

typedef unsigned short Offset;
typedef unsigned int Size;

struct RecordRef
{
	PageId page;
	Offset off;
};

struct Key
{
	const char* name;
	const void* key;
	size_t keylen;
};

struct IndexParam
{
	enum Type
	{
		Hash,
		BTree,
	} type;
	unsigned int maxkeylen;
	unsigned int hashtable;
	bool allowdup;
};

struct Page
{
	PageId id;
	PageInfo data;
};

class Cursor;
class Dbm;

class Index
{
protected:
	Dbm& m_db;
	Pager& m_pager;
	IndexParam m_param;
	Page m_main;
	Index(Dbm& db, Pager& pager, const IndexParam& param);
public:
	virtual void Add(RecordRef& rec, const void* key, size_t len) = 0;
	virtual Cursor* Select(const void* key, size_t len) = 0;
	Cursor* Select(const char* key);
};

typedef std::map<std::string, Index*> IndexMap;

struct LockInfo
{
	const void* ptr;
	size_t len;
};

class Dbm
{
	Pager& m_pager;
	IndexMap m_indexes;
public:
	Dbm(Pager& pager);
	~Dbm(void);
	Index* CreateIndex(const char* name, const IndexParam& param);
	Index* GetIndex(const char* name);
	const Index* GetIndex(const char* name) const;

	void Insert(const void* data, size_t len, RecordRef* ref, const Key** keys=NULL);
	void Lock(const RecordRef& rec,LockInfo* lock) const;

	void AddToIndex(RecordRef& rec, const Key& key);
	void AddToIndex(RecordRef& rec, const char* name, const void* key, size_t len);
	void AddToIndex(RecordRef& rec, const char* name, const char* key);
};

class Cursor
{
	const Dbm& m_db;
	// data
	LockInfo m_lock;
protected:
	Cursor(const Dbm& db);
	void SetData(const RecordRef& rec);
public:
	virtual bool Next() = 0;
	size_t GetRecSize();
	const void* Fetch();
	size_t Fetch(void* buff, size_t size);
	bool InMemory() { return true; }
	void Release() { delete this; }
};

} //namespace xcdb
