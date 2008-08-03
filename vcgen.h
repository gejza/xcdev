#pragma once
#include "generator.h"
#include "tool.h"

// zakladni tool, vytvari proj soubor
class VSSolGenerator : public Builder
{
	// file to out
public:
	void PushFolder(const char* name)
	{
		printf("filt %s\n", name);
	}
	void PopFolder()
	{
		printf("filt\n");
	}
	virtual unsigned long PassDependency() { return 0; }
};

// zakladni tool, vytvari proj soubor
class VSProjGenerator : public Builder
{
	// file to out
public:
	void PushFilter(const char* name)
	{
		printf("filt %s\n", name);
	}
	void PopFilter()
	{
		printf("filt\n");
	}
};

// tool, ktery umi konvertovat nastaveni
class VSProjTool : public Tool
{
	std::string m_path;
public:
	VSProjTool() {}
	virtual EObjectType ObjectUsed() { return EOTFile; } 
	virtual void Reset() {}
	virtual void AddSource(const char* src) { m_path = src; }
	virtual bool Build(IProperties* prop, IBuild*, ITool* link)
	{
		// set file
		printf("mgr file %s in conf %s\n", m_path.c_str(), prop->GetEnv("build"));
		if (link) link->AddSource((m_path+".o").c_str());
		return true;
	}
};

/////////////////////////////////////////
//
class VSFilter : public Filter
{
	std::string m_name;
public:
	VSFilter(Manager& mgr, const std::string& name)
		: Filter(mgr), m_name(name) {}
	virtual bool Make(IBuild* builder, ITarget* target, ITool* tool)
	{
		VSProjGenerator* vpg = dynamic_cast<VSProjGenerator*>(builder);
		vpg->PushFilter(m_name.c_str());
		bool ret = Filter::Make(builder, target, tool);
		vpg->PopFilter();
		return ret;
	}
};

class VSProject : public Project
{
	VSProjGenerator m_gen;
public:
	VSProject(Manager& mgr) : Project(mgr) {}
	virtual bool Make(IBuild* builder, ITarget* target, ITool* tool);
};

////////////////////////////////////////
// Gen
class VCGen :
	public Generator
{
public:
	VCGen(void);
	~VCGen(void);

	// tools?
	virtual Filter* CreateFilter(const char* name);
	virtual Project* CreateProject(const char* name);
	virtual Tool* GetTool(const char* name);
};
