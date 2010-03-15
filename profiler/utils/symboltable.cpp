
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "xc/symboltable.hpp"
#include "xc/error.h"
#include "cmdutil.h"

///////////////////////////////////
bool checkstatus(int status, const char* func)
{
	if (status != UDUMP_OK)
	{
		fprintf(stderr, "ERROR: Failed %s: %s\n", 
			func, udump_strerror(status));
		return false;
	}
	return true;
}

static void sym(symid_t id, const char* symbol, void*)
{
	printf("%d %s\n", id, symbol);
}

static int dump(int argc, const char* argv[])
{
	symboltable_dump(argv[0], sym, NULL);
	return 0;
}

static int info(int argc, const char* argv[])
{
	xc::SymbolTable_t table(argv[0], SYMTAB_READONLY|SYMTAB_FULLLOAD);
	symboltable_stat_t stat;
	table.stat(&stat);
	printf("Filename: %s\n", stat.filename);
	char u[64];
	xc_uuid_str(&stat.uuid, u);
	printf("UUID: %s\n", u);
	printf("Num symbols: %d\n", stat.count);
	printf("Pool blocks: %d\n", stat.pool_num);
	printf("Pool used: %d\n", stat.pool_used);
	printf("Pool free: %d\n", stat.pool_free);
	printf("Pool allocate: %d\n", stat.pool_alloc);
	return 0;
}

static int get(int argc, const char* argv[])
{
	xc::SymbolTable_t table(argv[0], 0);
	if (argc > 1)
	{
		while (++argv, --argc)
		{
			std::cout << table.get(argv[0]) << std::endl;
		}
	}
	else
	{
		std::string i;
		while (std::cin >> i, !std::cin.fail())
		{
			std::cout << table.get(i.c_str()) << std::endl;
		}
	}
	return 0;
}

static int translate(int argc, const char* argv[])
{
	xc::SymbolTable_t table(argv[0], SYMTAB_REVERSE|SYMTAB_READONLY|SYMTAB_FULLLOAD);
	if (--argc)
	{
		while (argc)
		{
			std::cout << table.translate(atoi(argv[0])) << std::endl;
			argc--; argv++;
		}
	}
	else
	{
		int i;
		while (std::cin >> i, !std::cin.fail())
		{
			std::cout << table.translate(i) << std::endl;
		}
	}
	return 0;
}

static const char* _default_symbols[] =
{
	"SystemInfo",
	"SymbolTableUUID", 
	"ThreadId",
	"Timing",
	NULL
};

static int create_default(int argc, const char* argv[])
{
	xc::SymbolTable_t table(argv[0], 0);
	for (int i=0; _default_symbols[i]; i++)
		table.get(_default_symbols[i]);
	return 0;
}

////////////////////////////////
DEFINE_COMMANDS
{
	"create", 1, 1, create_default,
		"create:    create <filename>\n"
		"           Create new table and put basic symbols.\n",
	"get", 1, -1, get,
		"get:       get <filename> [symbol] [symbol]...\n"
		"           Gets symbols from table.\n"
		"           Read from cmd line or stdin\n"
		"           If table not exist, create it.\n",
	"translate", 1, -1, translate, 
		"translate: translate <filename> [id] [id]...\n"
		"           Translate id to symbol.\n"
		"           Read from cmd line or stdin\n",
	"dump", 1, 1, dump,
		"dump:      dump <filename>\n"
		"           Print all symbols in table to stdout.\n"
		"           Format <id> <symbol> in one line\n",
	"info", 1, 1, info,
		"info:      info <filename>\n"
		"           Print info about table.\n",
	NULL
};






