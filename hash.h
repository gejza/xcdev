
namespace xcdb {

class Dbm;

typedef unsigned int HashNum;

struct HashItem {
	HashNum hash;
	RecordRef rec;
	RecordRef next;
	// 
	short keylen;
	short keysize;
};

class IndexHash : public Index
{
	PageId m_first; //todo do finalni struktury
public:
	IndexHash(Dbm& db, Pager& pager, const IndexParam& param);
	virtual void Add(RecordRef& rec, const void* key, size_t len);
	static HashNum Hash(const void* data, size_t len);
	virtual Cursor* Select(const void* key, size_t len);

	// metody pro cursory
	const HashItem* Find(const HashItem* item, HashNum hash) const;
};

class CursorHash : public Cursor
{
	const IndexHash& m_index;
	HashNum m_hash;
	const HashItem* m_item;
	const void* m_key;
	size_t m_len;
public:
	CursorHash(const Dbm& db, const IndexHash& index);
	void Select(const void* key, size_t len);
	virtual bool Next();
};

} // namespace xcdb
