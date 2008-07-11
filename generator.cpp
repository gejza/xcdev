#include "StdAfx.h"
#include "generator.h"
#include "properties.h"

Generator::Generator(void)
{
}

Generator::~Generator(void)
{
}

File* Generator::CreateFile(const char* path)
{
	return new File(*this, path);
}

Filter* Generator::CreateFilter(const char* name)
{
	return new Filter(*this);
}

Project* Generator::CreateProject(const char* name)
{
	return new Project(*this);
}

Folder* Generator::CreateFolder(const char* name)
{
	return new Folder(*this);
}

Solution* Generator::CreateSolution(const char* name)
{
	// vytvorit novou solution
	// solution jako folder?

	return new Solution(*this);
}

// tool
Tool* Generator::GetTool(const char* name)
{
	return 0;
}

Properties* Generator::NewProperties()
{
	return new Properties();
}

