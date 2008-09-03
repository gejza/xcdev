
#include "StdAfx.h"
#include "dbm.h"
#include "tree.h"
#include "dbutil.h"

namespace xcdb {

IndexTree::IndexTree(Dbm& db, Pager& pager, const IndexParam& param)
 : Index(db, pager, param)
{
}

void IndexTree::Add(RecordRef& rec, const void* key, size_t len)
{
}

Cursor* IndexTree::Select(const void* key, size_t len)
{
	return 0;
}

} // namespace xcdb
