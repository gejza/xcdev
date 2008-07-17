#include "StdAfx.h"
#include "object.h"
#include "generator.h"

/////////////////////////
// file


bool File::Make(ConfList& conf, IBuilder* bld, ITool* tool)
{
	Properties* prop = m_conf.Serialize(conf);
	for (Tools::iterator t=m_tool.begin();t!=m_tool.end();t++)
	{
		if (t==m_tool.begin())
			(*t)->AddSource(m_path.c_str());
		Tools::iterator n = t+1;
		if (!(*t)->Build(prop, bld, n != m_tool.end() ? *n:tool))
			return false;
	}
	return true;
}

void File::AddTool(const char* name)
{
	m_tool.push_back(m_gen.GetTool(name));
}

/////////////////////////
// filter
bool Filter::Make(ConfList& conf, IBuilder* bld, ITool* tool)
{
	bool res = true;
	// make folders, make projects -> podle dependency!!!
	for (Filter::Filters::iterator i=m_filters.begin();i != m_filters.end();i++)
		if (!(*i)->Make(conf, bld, tool))
			res = false;
	for (File::Files::iterator i=m_files.begin();i != m_files.end();i++)
		if (!(*i)->Make(conf, bld, tool))
			res = false;
	return res;
}

File* Filter::CreateFile(const char* path)
{
	m_files.push_back(m_gen.CreateFile(path));
	m_files.back()->SetOwner(this);
	return m_files.back();
}

Filter* Filter::CreateFilter(const char* name)
{
	m_filters.push_back(m_gen.CreateFilter(name));
	m_filters.back()->SetOwner(this);
	return m_filters.back();
}

/////////////////////////
// Project
bool Project::Make(ConfList& conf, IBuilder* bld, ITool* tool)
{
	bool res = true;
	// make folders, make projects -> podle dependency!!!
	for (Filter::Filters::iterator i=m_filters.begin();i != m_filters.end();i++)
		if (!(*i)->Make(conf, bld, m_link ? m_link:tool))
			res = false;
	for (File::Files::iterator i=m_files.begin();i != m_files.end();i++)
		if (!(*i)->Make(conf, bld, m_link ? m_link:tool))
			res = false;
	// link
	if (res && m_link)
	{
		Properties* prop = m_conf.Serialize(conf);
		res = m_link->Build(prop, bld, tool);
	}
	return res;
}

File* Project::CreateFile(const char* path)
{
	m_files.push_back(m_gen.CreateFile(path));
	m_files.back()->SetOwner(this);
	return m_files.back();
}

Filter* Project::CreateFilter(const char* name)
{
	m_filters.push_back(m_gen.CreateFilter(name));
	m_filters.back()->SetOwner(this);
	return m_filters.back();
}

void Project::SetLinker(const char* name)
{
	m_link = m_gen.GetTool(name);
}

/////////////////////////
// folder
bool Folder::Make(ConfList& conf, IBuilder* bld, ITool* tool)
{
	bool res = true;
	// make folders, make projects -> podle dependency!!!
	for (Folder::Folders::iterator i=m_folders.begin();i != m_folders.end();i++)
		if (!(*i)->Make(conf, bld, tool))
			res = false;
	for (Project::Projects::iterator i=m_projects.begin();i != m_projects.end();i++)
		if (!(*i)->Make(conf, bld, tool))
			res = false;
	// vygenerovat v toolu adresar a predat 
	return res;
}

File* Folder::CreateFile(const char* path)
{
	m_files.push_back(m_gen.CreateFile(path));
	m_files.back()->SetOwner(this);
	return m_files.back();
}

Project* Folder::CreateProject(const char* name)
{
	m_projects.push_back(m_gen.CreateProject(name));
	m_projects.back()->SetOwner(this);
	return m_projects.back();
}

Folder* Folder::CreateFolder(const char* name)
{
	m_folders.push_back(m_gen.CreateFolder(name));
	m_folders.back()->SetOwner(this);
	return m_folders.back();
}

/////////////////////////
// solution
File* Solution::CreateFile(const char* path)
{
	m_files.push_back(m_gen.CreateFile(path));
	m_files.back()->SetOwner(this);
	return m_files.back();
}

Project* Solution::CreateProject(const char* name)
{
	m_projects.push_back(m_gen.CreateProject(name));
	m_projects.back()->SetOwner(this);
	return m_projects.back();
}

Folder* Solution::CreateFolder(const char* name)
{
	m_folders.push_back(m_gen.CreateFolder(name));
	m_folders.back()->SetOwner(this);
	return m_folders.back();
}

bool Solution::Make(ConfList& conf, IBuilder* bld, ITool* tool)
{
	bool res = true;
	// make folders, make projects -> podle dependency!!!
	if (!m_folders.Make(conf, bld, tool))
		res = false;
	if (!m_projects.Make(conf, bld, tool))
		res = false;
	if (!m_files.Make(conf, bld, tool))
		res = false;
	return res;
}


