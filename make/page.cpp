#include "StdAfx.h"
#include "page.h"

struct PageData
{
	PageId id;
	size_t offset;
	size_t size;
};

struct PageHead
{
	PageId main;
	unsigned int numpages;
};

Pager::Pager(const char* pagefile) : m_lastId(0), m_sum(0), m_mainPage(0)
{
	m_file = fopen(pagefile, "rw");
	PageHead head;
	if (!fread(&head, sizeof(PageHead), 1, m_file))
		return;
	m_mainPage = head.main;
	if (!head.numpages)
		return;
	PageData* page = new PageData[head.numpages];
	head.numpages = fread(page, sizeof(PageData), head.numpages, m_file);
	for (unsigned int i=0;i < head.numpages;i++)
	{
		PageInfo p;
		p.ptr = (byte*)malloc(p.size = page[i].size);
		fseek(m_file, page[i].offset, SEEK_SET);
		fread(p.ptr, 1, p.size, m_file);
		m_pages[page[i].id] = p;
	}
	delete page;
}

Pager::~Pager(void)
{

}

PageId Pager::Create(size_t s, PageInfo* page, bool zero)
{
	m_lastId++;
	page->ptr = (byte*)malloc(page->size = s);
	m_sum += s;
	if (zero)
		memset(page->ptr, 0, s);
	m_pages.insert(std::make_pair(m_lastId, *page));
	return m_lastId;
}

void Pager::Get(PageId id, PageInfo* page)
{
	PageMap::iterator i = m_pages.find(id);
	assert(i != m_pages.end());
	*page = i->second;
}

void Pager::PrintStatus(FILE* f)
{
	fprintf(f, "-------------\n");
	fprintf(f, "Used: %ld\n", m_sum);
	fprintf(f, "Pages: %d\n", m_lastId);
	fprintf(f, "-------------\n");
}
