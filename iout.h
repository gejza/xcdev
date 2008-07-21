

#pragma once

typedef std::vector<std::string> ConfList;
// properties
// konfigurace, atd
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
};

// vyber architektury a platformy (pro uziti nastroju)
class IProfile
{
public:
};

enum EObjectType
{
	EOTFile = 0x1,
	EOTFilter = 0x3,
	EOTProject = 0x4,
	EOTFolder = 0x5,
	EOTSolution = 0x8,
};

class ITarget
{
public:
	virtual int GetLevel() = 0;
	virtual ITarget* GetNext(EObjectType type) = 0;
};

class IBuilder
{
public:
	virtual int BuildId() = 0;
	virtual ConfList& GetBuildConf() = 0;
	virtual IProfile* GetProfile() = 0; // ovlivnuji chovani nastroju
	virtual unsigned long PassDependency() = 0;
};

// tools
class ITool
{
public:
	virtual EObjectType ObjectUsed() = 0;
	virtual void Reset() = 0;
	virtual void AddSource(const char* src) = 0;
	virtual bool Build(IProperties* prop, IBuilder* builder, ITool* tool) = 0;
};

// objects
class IObject
{
public:
	virtual EObjectType GetType() = 0;
	virtual bool Make(IBuilder* builder, ITarget* target, ITool* tool) = 0;
	virtual IProperties* GetProperties(const char* name=NULL) = 0;
	// target spec
	virtual void AddDepend(ITarget* target, IObject* depend, ITarget* targetdep, bool direct) = 0;
	virtual void Include(ITarget* target, const char* conf) = 0;
	virtual void Exclude(ITarget* target, const char* conf) = 0;
	virtual void AddTool(ITarget* target, const char* name) = 0;
};

class IFile : public IObject
{
public:
	virtual EObjectType GetType() { return EOTFile; }
};

class IFilter : public IObject
{
public:
	virtual EObjectType GetType() { return EOTFilter; }
	virtual IFile* CreateFile(const char* path) = 0;
	virtual IFilter* CreateFilter(const char* name) = 0;
};

class IProject : public IFilter
{
public:
	virtual EObjectType GetType() { return EOTProject; }
	virtual IFile* CreateFile(const char* path) = 0;
	virtual IFilter* CreateFilter(const char* name) = 0;
};

class IFolder : public IObject
{
public:
	virtual EObjectType GetType() { return EOTFolder; }
	virtual IFile* CreateFile(const char* path) = 0;
	virtual IProject* CreateProject(const char* name) = 0;
	virtual IFolder* CreateFolder(const char* name) = 0;
};

class ISolution : public IFolder
{
public:
	virtual EObjectType GetType() { return EOTSolution; }
};

// generator
class IOutput
{
public:
	virtual IFile* CreateFile(const char* path) = 0;
	virtual IFilter* CreateFilter(const char* name) = 0;
	virtual IProject* CreateProject(const char* name) = 0;
	virtual IFolder* CreateFolder(const char* name) = 0;
	virtual ISolution* CreateSolution(const char* name) = 0;
	virtual ITool* GetTool(const char* name) = 0;
};
