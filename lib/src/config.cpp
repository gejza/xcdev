
#include <stdio.h>
#include <stdlib.h>
#include "xc/config.h"

XC_API int xcconfig_check(int apiversion, int threadsafe)
{
	if (apiversion != xc_api_version)
	{
		fprintf(stderr, "xclib: Version mismatch, binary: %d lib: %d.\n",
			apiversion, xc_api_version);
		abort();
	}
	if (threadsafe != XC_THREADSAFE)
	{
		fprintf(stderr, "xclib: Load %s library to %s binary.\n",
			XC_THREADSAFE ? "threadsafe":"nonsafe",
			threadsafe ? "threadsafe":"nonsafe");
		abort();
	}
	return 1;
}


