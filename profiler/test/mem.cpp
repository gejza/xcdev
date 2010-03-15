
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <new.h>
#include <stdlib.h>
#include <time.h>
#include "xcprof/mem.h"

// windbg
// _msize

int main()
{
	memdbg_start();
	new int[2];
	malloc(20);
	memdbg_end();
	return 0;
}
