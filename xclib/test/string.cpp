
#include <stdio.h>
#include <xc/string.hpp>
#include <xc/pool.hpp>
#include <xc/time.hpp>
#include <xc/debug.hpp>

const xc::String test()
{
	xc::String a,b,c;
	a = "helou";
	b = "lucky$(test)";
	c = "boy";
	xc::String t;
	t = a;
	a = b;
	b = c;
	c = t;
	return a + b + c + t;
}

class F : public xc::Filter
{
public:
	virtual bool Filt(xc::String& str) const
	{
		str = "'nahrazeno'";
		return true;
	}
};

const xc::String a()
{
	return xc::String("a");
}

int main()
{
	XC_VERSION_CHECK;
	xc::debug::EnableDebug(true);
	xc::String s(xc::String::ConstString("luc$ky$(test)neco"));
	//s.Replace("$\\(*\\)", F());
	printf("%s %s\n", s.Ptr(), a().Ptr());
	s += test();
	printf("%s\n", s.Ptr());

	return 0;
}

// string -> string data (dyn, const, pool, shmem)

