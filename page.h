#pragma once

typedef short PageId;
typedef unsigned char byte;

struct PageInfo
{
	byte* ptr;
	size_t size;
};

typedef std::map<PageId, PageInfo> PageMap;

class Pager
{
	PageId m_lastId;
	PageId m_mainPage;
	PageMap m_pages;
	size_t m_sum;

	FILE* m_file;
public:
	Pager(const char* pagefile);
	~Pager(void);
	PageId Create(size_t s, PageInfo* page=NULL, bool zero=false);
	void SetMainPage(PageId id);
	PageId GetMainPage();
	void Remove(PageId id);
	void Get(PageId id, PageInfo* page);
	void PrintStatus(FILE* f);
};
