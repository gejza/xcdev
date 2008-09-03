
namespace xcdb {

class IndexTree : public Index
{
public:
	IndexTree(Dbm& db, Pager& pager, const IndexParam& param);
	virtual void Add(RecordRef& rec, const void* key, size_t len);
	virtual Cursor* Select(const void* key, size_t len);
};

} // namespace xcdb
