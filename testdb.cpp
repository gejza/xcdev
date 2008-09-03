// testdb.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "dbm.h"
#include "page.h"

using namespace xcdb;

int _tmain(int argc, _TCHAR* argv[])
{
	Pager pager("page.db");
	Dbm dbm(pager);

	IndexParam idx;
	idx.type = IndexParam::Hash;
	idx.maxkeylen = 4;
	idx.allowdup = true;
	idx.hashtable = 0;
	dbm.CreateIndex("hash", idx);
	dbm.CreateIndex("hash2", idx);

	RecordRef rec;
	for (int i=0;i < 130;i++)
	{
		printf("%d  \r", i);
		char buff[1024];
		sprintf(buff, "Princatko cislo %d a %d", i, i << 2);
		dbm.Insert(buff, strlen(buff)+1, &rec);
		dbm.AddToIndex(rec, "hash", &i, 4);
		int x=i + 3;
		dbm.AddToIndex(rec, "hash2", &x, 4);
	}

	printf("Select\n");
	{
		int y=45;
		Cursor* c = dbm.GetIndex("hash")->Select(&y, 4);
		while (c->Next())
		{
			printf("%d: %s\n", y, c->Fetch());
		}
		c->Release();
	}
	{
		int y=45;
		Cursor* c = dbm.GetIndex("hash2")->Select(&y, 4);
		while (c->Next())
		{
			printf("%d: %s\n", y, c->Fetch());
		}
		c->Release();
	}
	pager.PrintStatus(stdout);
	return 0; 
}

