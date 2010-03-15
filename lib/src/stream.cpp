
#include <stdio.h>
#include "xc/stream.h"
#include "xc/string.h"

/*XC_API const xc::ConstString xc::Stream::ToString() const
{
	char buff[5];
	size_t s = this->Read(buff, sizeof(buff));
	if (!s)
		return "";
	xc::String d(buff, s);
	while (s = this->Read(buff, sizeof(buff)))
	{
		d.Append(buff, s);
	}
	return d;
}*/

/*
XC_API size_t xc::StreamI::Read(char* buff, size_t num) const
{
	if (!p)
		return 0;
	size_t m = p;
	if (m > num) m = num;
	memset(buff, 'a', m);
	p -= m;
	return m;
}*/
