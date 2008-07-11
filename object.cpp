#include "StdAfx.h"
#include "object.h"
#include "generator.h"

/////////////////////////
// file
bool File::Make(ConfList& conf)
{
	// spustit prikazovou radku pro soubor
	IProperties* prop = m_conf.Serialize(conf);
	printf("Make file %s. - %s\n", m_path.c_str(), 
		prop ? prop->GetEnv("build"):"no prop");

	return 0;
}

/////////////////////////
// filter
bool Filter::Make(ConfList& conf)
{
	bool res = true;
	// make folders, make projects -> podle dependency!!!
	for (Filter::Filters::iterator i=m_filters.begin();i != m_filters.end();i++)
		if (!(*i)->Make(conf))
			res = false;
	for (File::Files::iterator i=m_files.begin();i != m_files.end();i++)
		if (!(*i)->Make(conf))
			res = false;
	return res;
}

File* Filter::CreateFile(const char* path)
{
	m_files.push_back(m_gen.CreateFile(path));
	m_files.back()->SetParentConf(&m_conf);
	return m_files.back();
}

Filter* Filter::CreateFilter(const char* name)
{
	m_filters.push_back(m_gen.CreateFilter(name));
	m_filters.back()->SetParentConf(&m_conf);
	return m_filters.back();
}

/////////////////////////
// Project
bool Project::Make(ConfList& conf)
{
	bool res = true;
	// make folders, make projects -> podle dependency!!!
	for (Filter::Filters::iterator i=m_filters.begin();i != m_filters.end();i++)
		if (!(*i)->Make(conf))
			res = false;
	for (File::Files::iterator i=m_files.begin();i != m_files.end();i++)
		if (!(*i)->Make(conf))
			res = false;
	return res;
}

File* Project::CreateFile(const char* path)
{
	m_files.push_back(m_gen.CreateFile(path));
	m_files.back()->SetParentConf(&m_conf);
	return m_files.back();
}

Filter* Project::CreateFilter(const char* name)
{
	m_filters.push_back(m_gen.CreateFilter(name));
	m_filters.back()->SetParentConf(&m_conf);
	return m_filters.back();
}

/////////////////////////
// folder
bool Folder::Make(ConfList& conf)
{
	bool res = true;
	// make folders, make projects -> podle dependency!!!
	for (Folder::Folders::iterator i=m_folders.begin();i != m_folders.end();i++)
		if (!(*i)->Make(conf))
			res = false;
	for (Project::Projects::iterator i=m_projects.begin();i != m_projects.end();i++)
		if (!(*i)->Make(conf))
			res = false;
	return res;
}

File* Folder::CreateFile(const char* path)
{
	m_files.push_back(m_gen.CreateFile(path));
	m_files.back()->SetParentConf(&m_conf);
	return m_files.back();
}

Project* Folder::CreateProject(const char* name)
{
	m_projects.push_back(m_gen.CreateProject(name));
	m_projects.back()->SetParentConf(&m_conf);
	return m_projects.back();
}

Folder* Folder::CreateFolder(const char* name)
{
	m_folders.push_back(m_gen.CreateFolder(name));
	m_folders.back()->SetParentConf(&m_conf);
	return m_folders.back();
}

/////////////////////////
// solution
File* Solution::CreateFile(const char* path)
{
	m_files.push_back(m_gen.CreateFile(path));
	m_files.back()->SetParentConf(&m_conf);
	return m_files.back();
}

Project* Solution::CreateProject(const char* name)
{
	m_projects.push_back(m_gen.CreateProject(name));
	m_projects.back()->SetParentConf(&m_conf);
	return m_projects.back();
}

Folder* Solution::CreateFolder(const char* name)
{
	m_folders.push_back(m_gen.CreateFolder(name));
	m_folders.back()->SetParentConf(&m_conf);
	return m_folders.back();
}

bool Solution::Make(ConfList& conf)
{
	bool res = true;
	// make folders, make projects -> podle dependency!!!
	for (::Folder::Folders::iterator i=m_folders.begin();i != m_folders.end();i++)
		if (!(*i)->Make(conf))
			res = false;
	for (Project::Projects::iterator i=m_projects.begin();i != m_projects.end();i++)
		if (!(*i)->Make(conf))
			res = false;
	for (File::Files::iterator i=m_files.begin();i != m_files.end();i++)
		if (!(*i)->Make(conf))
			res = false;
	return res;
}


