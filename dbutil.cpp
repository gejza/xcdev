
#include "StdAfx.h"
#include "dbm.h"
#include "dbutil.h"

namespace xcdb {

PageManager::PageManager(Pager& pager, const PageId id)
{
	PageInfo page;
	pager.Get(id, &page);
	m_state = reinterpret_cast<PageState*>(page.ptr);
	//todo asert za behu
	assert(m_state->size == page.size);
}

PageManager::PageManager(Pager& pager, PageId* id, size_t size)
{
	PageInfo page;
	*id = NewPage(pager, size, &page);
	m_state = reinterpret_cast<PageState*>(page.ptr);
}

PageId PageManager::NewPage(Pager& pager, Size datasize, PageInfo* info)
{
	PageInfo page;
	PageId id = pager.Create(sizeof(PageState)+datasize, &page);
	InitPage(*reinterpret_cast<PageState*>(page.ptr), page.size);
	if (info) *info = page;
	return id;
}

void PageManager::InitPage(PageState& page, Size size)
{
	memset(&page, 0, sizeof(PageState));
	page.used = sizeof(PageState);
	page.size = size;
	page.free = page.size - page.used;
}

bool PageManager::Alloc(Size s, RecordPtr* rec)
{
	if (m_state->free < s)
		return false;
	// preferovat freelist nebo vklad nakonec?
	const Size fe = m_state->size - m_state->used;
	const Size fl = m_state->free - fe;
	return AllocFromEnd(s, rec);
}

bool PageManager::AllocFromEnd(Size size, RecordPtr* rec)
{
	if (GetEndSpace() < size)
		return false;
	// vytvorit zaznam
	rec->off = m_state->used;
	rec->ptr = Case(m_state->used);
	//todo (je zbytecne uchovavat poslednich x bytu)
	rec->size = size;
	m_state->used += size;
	m_state->free -= size;
	return true;
}

} // namespace xcdb
