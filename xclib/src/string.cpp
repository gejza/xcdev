
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <vector>
#if 0

#include "xc/string.hpp"
#include "xc/stream.hpp"
#include "xc/debug.hpp"

class ReferencedData : public xc::StringData
{
	int m_ref;
public:
	ReferencedData() : m_ref(1) {}
	virtual ~ReferencedData()
	{
		XC_DBG(1, "Delete %p", this);
	}
	virtual void Lock()
	{ 
		XC_DBG(1, "Add reference %p(%s)", this, this->Data());
		m_ref++;
	}
	virtual void Unlock()
	{
		XC_DBG(1, "Dec reference %p(%s)", this, this->Data());
		if (!--m_ref)
			delete this;
	}
};

template<typename DataAllocator>
class CaseSensitiveData : public DataAllocator
{
public:
	template<typename ARG1>
	CaseSensitiveData(ARG1 arg1) : DataAllocator(arg1) {}
	template<typename ARG1, typename ARG2>
	CaseSensitiveData(ARG1 arg1, ARG2 arg2) : DataAllocator(arg1, arg2) {}
	virtual int Compare(const char* data) const
	{
		return strcmp(this->Data(), data);
	}
};

class StringDataConst : public ReferencedData
{
	const char* m_ptr;
public:
	StringDataConst(const char* ptr) : m_ptr(ptr) {}
	virtual ~StringDataConst() {}
	virtual const char* Data() const { return m_ptr; }
	virtual size_t Size() const { return xc_strlen(m_ptr); }
	virtual StringData* Rewrite(size_t ext)
	{
		return StringData::Alloc(m_ptr, xc_strlen(m_ptr) + ext);
	}
	virtual void Append(const char* ptr, size_t size)
	{
		// error
	}
};

class StringDataMem : public ReferencedData
{
	char* m_ptr;
public:
	StringDataMem(const char* ptr, size_t size)
		: m_ptr(NULL)
	{
		size_t min = xc_strlen(ptr) + size + 1;
		m_ptr = (char*) malloc(min);
		strcpy(m_ptr, ptr);
	}
	virtual ~StringDataMem() { free((void*)m_ptr); }
	virtual const char* Data() const { return m_ptr; }
	virtual size_t Size() const { return xc_strlen(m_ptr); }
	virtual StringData* Rewrite(size_t ext)
	{
		return StringData::Alloc(m_ptr, xc_strlen(m_ptr) + ext);
	}
	virtual void Append(const char* ptr, size_t size)
	{
		size_t s = xc_strlen(m_ptr);
		xc::debug::Print(1, "Append `%s' < `%s'", m_ptr, ptr);
		strcpy(m_ptr + s, ptr);
	}
};

class PrivateString : public xc::String
{
public:
	PrivateString(xc::StringData* data)
	{
		m_str = data;
	}
};

///////////////////////////////////////////
// data
XC_API xc::StringData* xc::StringData::Const(const char* data)
{
	return new CaseSensitiveData<StringDataConst>(data);
}

XC_API xc::StringData* xc::StringData::Alloc(const char* data, size_t size)
{
	return new CaseSensitiveData<StringDataMem>(data, size);
}

/////////////////////////////////
XC_API const xc::String xc::String::ConstString(const char* str)
{
	return PrivateString(xc::StringData::Const(str));
}

/*XC_API xc::ConstString::ConstString(const xc::Stream &str) : data(0)
{
	xc_string_ref(&data, str.ToString().data);
}

int xc_strfind(const char* str, char c)
{
	for (int i=0; str[i];i++)
		if (str[i] == c)
			return i;
	return -1;
}

XC_API int xc::String::Replace(const char* ,const Filter& filter)
{
	int rep = 0;
	xc::String n;
	int f,e;
	const char* ptr = this->Ptr();
	size_t s = this->Size();
	while (s && (f = xc_strfind(ptr, '$')) != -1)
	{
		if (ptr[f+1] == '(' && (e = xc_strfind(ptr+f+2, ')')) != -1)
		{
			e += f+2;
			if (f)
				n.Append(ptr, f);
			xc::String nn(ptr+f+2, e-2-f);
			filter.Filt(nn);
			n.Append(nn);
			ptr += e+1;
			s -= e+1;
			rep++;
		}
		else
		{
			n.Append(ptr, ++f);
			ptr+=f; s-=f;
		}
	}
	if (rep)
	{
		n.Append(ptr, s);
		this->Set(n);
		return rep;
	}
	return 0;
}
*/

#endif

