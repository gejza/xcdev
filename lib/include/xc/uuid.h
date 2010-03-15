
#pragma once
#ifndef _XC_UUID_H_
#define _XC_UUID_H_

#include "string.h"

namespace xc {

struct uuid
{
    xcuint32_t data1;
    xcuint16_t data2;
    xcuint16_t data3;
    xcbyte_t data4[8];

	static int compare(const uuid& id1, const uuid& id2);
	
	bool operator == (const uuid& ref) const
	{
		return compare(*this, ref) == 0;
	}
	bool operator != (const uuid& ref) const
	{
		return compare(*this, ref) != 0;
	}
	bool operator < (const uuid& ref) const
	{
		return compare(*this, ref) > 0;
	}
	bool operator > (const uuid& ref) const
	{
		return compare(*this, ref) < 0;
	}

};

} // namespace xc

#endif // _XC_UUID_H_
