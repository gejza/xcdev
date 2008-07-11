#pragma once

#include "iout.h"
#include "object.h"
#include "tool.h"
#include "properties.h"

class Generator : public IGenerator
{
public:
	Generator(void);
	~Generator(void);
	// gen
	virtual File* CreateFile(const char* path);
	virtual Filter* CreateFilter(const char* name);
	virtual Project* CreateProject(const char* name);
	virtual Folder* CreateFolder(const char* name);
	virtual Solution* CreateSolution(const char* name);
	virtual Tool* GetTool(const char* name);
	virtual Properties* NewProperties();
};
