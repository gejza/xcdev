#include "StdAfx.h"
#include "tool.h"

///////////////////////////////
// Builder
int Builder::BuildId()
{
	assert(m_parent);
	return m_parent->BuildId();
}

ConfList& Builder::GetBuildConf()
{
	assert(m_parent);
	return m_parent->GetBuildConf();
}

IProfile* Builder::GetProfile()
{
	assert(m_parent);
	return m_parent->GetProfile();
}

unsigned long Builder::PassDependency()
{
	assert(m_parent);
	return m_parent->PassDependency();
}

///////////////////////////////
//
Tool::Tool(void)
{
}

Tool::~Tool(void)
{
}
