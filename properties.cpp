#include "StdAfx.h"
#include "properties.h"
#include "generator.h"

Properties::Properties() : m_parent(NULL)
{
}

Properties::~Properties(void)
{
}

void Properties::Set(const char* section, const char* prop, const char* str)
{
}

const char* Properties::GetString(const char* section, const char* prop)
{
	return 0;
}

void Properties::Remove(const char* section, const char* prop)
{
}

const char* Properties::GetEnv(const char* name)
{
	EnvMap::iterator e = m_env.find(name);
	if (e != m_env.end())
		return e->second.c_str();
	if (m_parent)
		return m_parent->GetEnv(name);
	return NULL;
}

void Properties::SetEnv(const char* name, const char* value)
{
	m_env[name] = value;
}

bool Properties::IsSelect()
{
	return true;
}

void Properties::SetParent(Properties* prop)
{
	m_parent = prop;
}

/////////////////////////////////////////////////////
Properties* Configurations::Get(const char* conf)
{
	if (!conf) // main configuration
	{
		if (!m_main)
			m_main = m_gen.NewProperties();
		return m_main;
	}
	PropMap::iterator c = m_optional.find(conf);
	if (c != m_optional.end())
		return c->second;
	Properties* r = m_gen.NewProperties();
	m_optional[conf] = r;
	return r;
}

Properties* Configurations::Serialize(ConfList& conflist)
{
	// dedicnost
	Properties* p = m_parent ? m_parent->Serialize(conflist):NULL;
	// hlavni
	if (m_main)
	{
		if (p) m_main->SetParent(p);
		p = m_main;
	}	
	// volitelne
	if (m_optional.empty()) return p; // opt
	for (ConfList::iterator i=conflist.begin();i!=conflist.end();i++)
	{
		PropMap::iterator c = m_optional.find(*i);
		if (c != m_optional.end())
		{
			if (p) c->second->SetParent(p);
			p = c->second;
		}
	}
	return p;	
}


