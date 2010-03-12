
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <map>
#include <string>
#include "xc/tree.hpp"
#include "xc/error.hpp"
//#include "xcprof/mem.h"
//#include "xcprof/times.h"

class StdTreeDB
{
	std::map<std::string, int> tree;
public:
	void Insert(const char* k, int d)
	{
		tree[std::string(k)] = d;
	}
	int Search(const char* k)
	{
		std::map<std::string, int>::iterator i = tree.find(std::string(k));
		return i != tree.end() ? i->second:0;
	}
	void Stat()
	{
	}
	
};

const char* getk(int k)
{
	static char buff[32];
	sprintf(buff,"%d", k);
	return buff;
}

extern "C" {
extern size_t memalloc;
}

template<typename DB> size_t test(int entries, int searches)
{
	{
		DB db;
	//time_watch_t ins, srch;
	
	//memdbg_start();
	//watch_start(&ins);
	for (int i=1;i < entries;i++)
	{
		db.Insert(getk(i), i+1);
	}
	//watch_finish(&ins, entries);

	//watch_start(&srch);
	//for (int j=0;j < 30;j++)
		for (int i=1;i < searches;i++)
		{
			db.Search(getk(i%entries));
		}
	//watch_finish(&srch, searches);

	//printf("Insert: "); watch_report(&ins, stdout);
	//printf(" Search: "); watch_report(&srch, stdout); putchar('\n');
	db.Stat();
}
	//memdbg_end();
	return 0;
}

void tst(int entries, int searches)
{
	printf("#### Test for %d inserts and %d searches\n", entries, searches);
	printf("Std----\n");
	test<StdTreeDB>(entries, searches);
	printf("Tree----\n");
	test<xc::Tree>(entries, searches);
}

#ifdef _DEBUG
#define IDEAL(n) (n / 10)
#else
#define IDEAL(n) (n * 10)
#endif

void xc_exception_handler(int status, const char* msg)
{
	throw xc::exception(status, msg);
}

int main()
{
	xc_error_sethandler(xc_exception_handler, "exception handler");
	tst(IDEAL(100),IDEAL(100));
	tst(IDEAL(100),IDEAL(1000));
	tst(IDEAL(100),IDEAL(10000));
	tst(IDEAL(10000),IDEAL(1000));
	tst(IDEAL(10000),IDEAL(10000));
	tst(IDEAL(10000),IDEAL(100000));
	return 0;
}


