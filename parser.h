#pragma once

#include "iparser.h"

// stavovy parser
class Parser : public IParser
{
	// zasobnik pro objekty
public:
	Parser(void);
	~Parser(void);

	virtual void AddObj(int type, const char* name) { printf("Obj: %s\n", name); }
	virtual void Option(const char* name) { printf("Opt: %s\n", name); }
	virtual void Cmd(const char* name) { printf("Cmd: %s\n", name); }
	virtual void Arg(bool b)  { printf("Bool: %d\n", (int)b); }
	virtual void Arg(const char* s) { printf("Arg: %s\n", s); }

	virtual void End() { printf("End\n"); }
};



