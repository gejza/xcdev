#include "StdAfx.h"
#include "vcgen.h"
#include "tool.h"

class VSProject;

// zakladni tool, vytvari proj soubor
class VSProjGenerator : public IBuilder
{
public:
};

// tool, ktery umi konvertovat nastaveni
class VSProjTool : public Tool
{
public:
	VSProjTool() {}
	virtual void Reset() {}
	virtual void AddSource(const char* src) {}
	virtual bool Build(IProperties* prop, IBuilder*, ITool* link)
	{
		printf("gen file\n");
		return true;
	}
};

/////////////////////////////////////////
//
class VSFile : public File
{
public:
	VSFile(Generator& gen, const char* path) : File(gen, path) {}
	virtual bool Make(ConfList& conf, ITool* exp, IObject* req)
	{
		return false;
	}
};

class VSFilter : public Filter
{
public:
	VSFilter(Generator& gen) : Filter(gen) {}
};

class VSProject : public Project
{
	VSProjGenerator m_gen;
public:
	VSProject(Generator& gen) : Project(gen) {}
	virtual bool Make(ConfList& conf, IBuilder* bld, ITool* tool)
	{
		return Project::Make(conf, bld, tool);
	}
};

///////////////////////////////////////////
// VSGen
VCGen::VCGen(void)
{
}

VCGen::~VCGen(void)
{
}

File* VCGen::CreateFile(const char* path)
{
	return new VSFile(*this, path);
}

Filter* VCGen::CreateFilter(const char* name)
{
	return new VSFilter(*this);
}

Project* VCGen::CreateProject(const char* name)
{
	return new VSProject(*this);
}

Tool* VCGen::GetTool(const char* name)
{
	return new VSProjTool();
}

// gen = linker
// cc = 

