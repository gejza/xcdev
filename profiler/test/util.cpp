
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <new.h>
#include <stdlib.h>
#include <time.h>
#include "xc/uuid.h"

int main()
{
	xc_uuid_t uuid;
	char buff[128];
	for (int i=0; i < 10; i++)
	{
		xc_uuid_create(&uuid);
		xc_uuid_str(&uuid, buff);
		printf("uuid=%s\n", buff);
	}

	return 0;
}
