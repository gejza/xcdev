

#pragma once

typedef std::vector<std::string> ConfList;

// properties
// architektura, platforma, konfigurace, atd
class IProperties
{
public:
	// seters
	virtual void Set(const char* section, const char* prop, const char* str) = 0;
	// geters
	virtual const char* GetString(const char* section, const char* prop) = 0;
	// others
	virtual void Remove(const char* section, const char* prop) = 0;
	virtual const char* GetEnv(const char* name) = 0;
	virtual void SetEnv(const char* name, const char* value) = 0;
	virtual bool IsSelect() = 0; //< selector pro skupiny
};

// tools
class ITool
{
public:
};

// objects
class IObject
{
public:
	virtual bool Make(ConfList& conf) = 0;
	virtual IProperties* GetProperties(const char* name=NULL) = 0;
};

class IFile : public IObject
{
};

class IFilter : public IObject
{
public:
	virtual IFile* CreateFile(const char* path) = 0;
	virtual IFilter* CreateFilter(const char* name) = 0;
};

class IProject : public IFilter
{
public:
	virtual IFile* CreateFile(const char* path) = 0;
	virtual IFilter* CreateFilter(const char* name) = 0;
};

class IFolder : public IObject
{
public:
	virtual IFile* CreateFile(const char* path) = 0;
	virtual IProject* CreateProject(const char* name) = 0;
	virtual IFolder* CreateFolder(const char* name) = 0;
};

class ISolution : public IFolder
{
};

// generator
class IGenerator
{
public:
	virtual IFile* CreateFile(const char* path) = 0;
	virtual IFilter* CreateFilter(const char* name) = 0;
	virtual IProject* CreateProject(const char* name) = 0;
	virtual IFolder* CreateFolder(const char* name) = 0;
	virtual ISolution* CreateSolution(const char* name) = 0;

	// tool
	virtual ITool* GetTool(const char* name) = 0;
};
