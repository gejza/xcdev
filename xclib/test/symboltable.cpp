
#include <stdio.h>
#include <string.h>
#include "xc/symboltable.hpp"
#include "xc/error.h"

static const char* test_filename = "table.sym";
static const char* saved_symbol = "saved symbol";
static const int test_translate_min = 8;
static const int test_translate_size = 512;

bool test(xc::SymbolTable& table, const char* symbol)
{
	printf("# test symbol '%s'\n", symbol);
	// test insert symbol
	symid_t id = table.Get(symbol);
	if (!id)
	{
		fprintf(stderr, "ERROR: Symbol '%s' failed insert.\n", symbol);
		return false;
	}

	// test other id than ref
	symid_t idref = table.Get("other insert symbol");
	if (id == idref)
	{
		fprintf(stderr, "ERROR: Symbol '%s' dublicate id %d.\n", symbol, id);
		return false;
	}

	// test repeatinsert 
	symid_t id2 = table.Get(symbol);
	if (id != id2)
	{
		fprintf(stderr, "ERROR: Symbol '%s' mismatch id %d newid=%d.\n",
			symbol, id, id2);
		return false;
	}


	// test translate symbol
	char str[test_translate_size];
	size_t symsize = strlen(symbol) + 1;
	size_t sn = symboltable_translate(table, id, str, test_translate_min);
	// small buffer
	if (sn > test_translate_min)
	{
		if (sn != symsize)
		{
			fprintf(stderr, "ERROR: Return invalid symbol size. "
				            "Must %d and not %d\n", symsize, sn);
			return false;
		}
		sn = symboltable_translate(table, id, str, sizeof(str));
		if (sn > sizeof(str))
		{
			fprintf(stderr, "TEST ERROR: Small test bufer, requied %d\n", sn);
			return false;
		}
	}
	if (!sn)
	{
		fprintf(stderr, "ERROR: Symbol %d(%s) not found for translate.\n", id, symbol);
		return false;
	}
	if (sn != symsize)
	{
		fprintf(stderr, "ERROR: Return invalid symbol size. "
			            "Must %d and not %d\n", symsize, sn);
		return false;
	}
	if (str[sn-1])
	{
		fprintf(stderr, "ERROR: Translate symbol %d(%s) missing 0 on end.\n", id, symbol);
		return false;
	}
	if (strcmp(symbol, str) != 0)
	{
		fprintf(stderr, "ERROR: Translate symbol %d(%s) as '%s'.\n", id, symbol, str);
		return false;
	}
	return true;
}

bool checkstatus(int status, const char* func)
{
	if (status != XC_OK)
	{
		fprintf(stderr, "ERROR: Failed %s '%s': %s\n", 
			func, test_filename, xc_error_string(status));
		return false;
	}
	return true;
}

const char* test_sym[] = {
	"Symbol1",
	"Symbol with space",
	"!@#$%^&*()",
	"Very very very very very very very very very long symbol",
	"Symbol2",
	0,
};

int main()
{
	//return checkstatus(symboltable_dump(test_filename, sym, NULL), "dump");

	xc::SymbolTable table(test_filename, SYMTAB_REVERSE);
	symid_t saveid, saveid2;

	// test ok
	for (int i=0; test_sym[i]; i++)
		if (!test(table, test_sym[i]))
			goto err;

	// test errors
	{
		if (table.Get(NULL))
		{
			fprintf(stderr, "ERROR: Positive id when insert NULL\n");
			goto err;
		}
		if (table.Get(NULL))
		{
			fprintf(stderr, "ERROR: Positive id when insert empty string\n");
			goto err;
		}
	}

	// insert for after reopen
	saveid = table.Get(saved_symbol);
	// insert many symbols
	{
		for (int j=0;j < 1000;j++)
		{
			char buff[32];
			sprintf(buff, "sym-%d", (j+1));
			if (!symboltable_get(table, buff))
			{
				fprintf(stderr, "ERROR: Failed insert symbol '%s'\n", buff);
				goto err;
			}
		}
	}

	// test not translate symbol
	{
		char tmp[512];
		size_t nfs = symboltable_translate(table, ~((symid_t)1), tmp, sizeof(tmp));
		if (nfs)
		{
			tmp[511] = 0;
			fprintf(stderr, "ERROR: Translate invalid symid positive '%s'\n", tmp);
				goto err;

		}
	}

	// get stat
	symboltable_stat_t stat;
	symboltable_stat(table, &stat);
	printf("--- Stats ---\n");
	printf("File: %s\n", stat.filename);
	printf("Count symbol: %d\n", stat.count);
	printf("Num blocks: %d\n", stat.pool_num);
	printf("Pool used: %ld\n", stat.pool_used);
	printf("Pool free: %ld\n", stat.pool_free);
	printf("Pool alloc: %ld\n", stat.pool_alloc);


	printf("Closing table...\n");
	if (!checkstatus(symboltable_close(table), "close"))
		return 2;

	// test reopen
	printf("Test reopen...");
	if (!checkstatus(symboltable_init(table, test_filename, SYMTAB_READONLY), "reopen"))
		return 2;

	saveid2 = symboltable_get(table, saved_symbol);
	if (saveid != saveid2)
	{
		fprintf(stderr, "ERROR: Symbol '%s' mismatch after save id %d newid=%d.\n",
			saved_symbol, saveid, saveid2);
		goto err;
	}

	if (!checkstatus(symboltable_close(table), "close2"))
		return 2;

	// test only in memory
	printf("Test table in memory\n");
	if (!checkstatus(symboltable_init(table, NULL, SYMTAB_REVERSE), "open in mem"))
		return 2;

	for (int i=0; test_sym[i]; i++)
		if (!test(table, test_sym[i]))
			goto err;

	if (!checkstatus(symboltable_close(table), "close2"))
		return 2;

	printf("Test OK!\n");

	return 0;

err:
	printf("Closing table...\n");
	checkstatus(symboltable_close(table),"close after error");
	return 1;
}
