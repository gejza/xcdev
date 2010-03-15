#pragma once

#include "iparser.h"

// stavovy parser
// ovlada konfigurator

// konfigurator
//   - vytvari zavislosti, konfigurace, objekty

// generator
//   - generuje konkretni projektove soubory nebo skripty

class Parser : public IParser
{
	// zasobnik pro objekty
	// 
public:
	Parser(void);
	~Parser(void);

	virtual void AddObj(int type, const xcstring& name) { printf("Obj: %s\n", name.str); }
	virtual void Option(const char* name) { printf("Opt: %s\n", name); }
	virtual void Cmd(const char* name) { printf("Cmd: %s\n", name); }
	virtual void Arg(bool v) { printf("bool\n"); }
	virtual void Arg(const xcstring& v) { printf("Arg: %s\n", v.str); }
	virtual void Arg(const xcnumber& n) { printf("Arg: %ld\n", n.n); }

	virtual void End() { printf("End\n"); }
	virtual void Error() { printf("Error!!!\n"); }
};



