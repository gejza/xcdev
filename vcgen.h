#pragma once
#include "generator.h"

class VCGen :
	public Generator
{
public:
	VCGen(void);
	~VCGen(void);

	// gen solution
	// gen project

	// tools?
	virtual File* CreateFile(const char* path);
	virtual Filter* CreateFilter(const char* name);
	virtual Project* CreateProject(const char* name);
	virtual Tool* GetTool(const char* name);
};
