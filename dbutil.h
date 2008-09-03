
namespace xcdb {

struct PageState
{
	PageId prev_page;
	PageId next_page;
	Offset size;
	Offset used;
	Offset freelist;
	Size free;
};

struct RecordPtr
{
	Offset off;
	byte* ptr;
	size_t size;
};

class PageManager
{
	PageState* m_state;
protected:
	static void InitPage(PageState& page, Size size);
	bool AllocFromFreelist(Size size, RecordPtr* rec, bool optimize);
	bool AllocFromEnd(Size size, RecordPtr* rec);
	// tests
	inline const Size GetEndSpace() const 
	       { return m_state->size - m_state->used; }
	inline const Size GetFreelistSpace() const 
	       { return m_state->free - GetEndSpace(); }
public:
	inline PageManager(byte* base)
	{
		m_state = reinterpret_cast<PageState*>(base);
	}
	inline PageManager(const PageInfo& page)
	{
		m_state = reinterpret_cast<PageState*>(page.ptr);
		assert(m_state->size == page.size);
	}
	PageManager(Pager& pager, const PageId id);
	PageManager(Pager& pager, PageId* id, size_t size);
	inline PageState& State() { return *m_state; }
	static PageId NewPage(Pager& pager, Size size, PageInfo* info = NULL);
	bool Alloc(Size size, RecordPtr* rec);
	inline byte* Case(Offset off) { return reinterpret_cast<byte*>(m_state) + off; }
};

template<typename T>
T byte_cast(void* ptr, Offset off)
{
	return reinterpret_cast<T>(reinterpret_cast<char*>(ptr)+off);
}

} // namespace xcdb
