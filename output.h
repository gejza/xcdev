#pragma once

#include "object.h"
#include "generator.h"

class Output : public Generator
{
public:
	Output(void);
	~Output(void);

	//virtual Solution* CreateSolution(const char* name);
};
