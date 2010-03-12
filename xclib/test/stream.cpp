
#include <stdio.h>
#include <xc/string.hpp>
#include <xc/pool.hpp>
#include <xc/time.hpp>

int main()
{
	XC_VERSION_CHECK;
	
	xc::CStream s;
	xc::ConstString ss(s);
	printf("%s\n", ss.Ptr());
	return 0;
}

// string -> string data (dyn, const, pool, shmem)

