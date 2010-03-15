#include "StdAfx.h"
#include "vcgen.h"
#include "tool.h"

///////////////////////////////////////////
// VSProject
bool VSProject::Make(IBuild* bld, ITarget* target, ITool* tool)
{
	// nahradit build
	// vytvorit v build
	m_gen.Set(bld);
	// 
	return Project::Make(&m_gen, target, tool);
}

///////////////////////////////////////////
// VSGen
VCGen::VCGen(void)
{
}

VCGen::~VCGen(void)
{
}

Filter* VCGen::CreateFilter(const char* name)
{
	return new VSFilter(*this, name);
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

