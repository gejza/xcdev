#pragma once

#include "iout.h"
#include "properties.h"

class Generator;

class File : public IFile
{
public:
	typedef std::vector<File*> Files;
protected:
	Generator& m_gen;
	std::string m_path;
	Configurations m_conf;
public:
	File(Generator& gen, const char* path) 
		: m_gen(gen), m_path(path), m_conf(gen) {}
	virtual bool Make(ConfList& conf);
	virtual IProperties* GetProperties(const char* conf) { return m_conf.Get(conf); }
	void SetParentConf(Configurations* parent) { m_conf.SetParent(parent); }
};

class Filter : public IFilter
{
public:
	typedef std::vector<Filter*> Filters;
protected:
	Generator& m_gen;
	Filter::Filters m_filters;
	File::Files m_files;
	Configurations m_conf;
public:
	Filter(Generator& gen) : m_gen(gen), m_conf(gen) {}
	virtual File* CreateFile(const char* path);
	virtual Filter* CreateFilter(const char* name);
	virtual bool Make(ConfList& conf);
	virtual IProperties* GetProperties(const char* conf) { return m_conf.Get(conf); }
	void SetParentConf(Configurations* parent) { m_conf.SetParent(parent); }
};

class Project : public IProject
{
public:
	typedef std::vector<Project*> Projects;
protected:
	Generator& m_gen;
	Filter::Filters m_filters;
	File::Files m_files;
	Configurations m_conf;
public:
	Project(Generator& gen) : m_gen(gen), m_conf(gen) {}
	virtual File* CreateFile(const char* path);
	virtual Filter* CreateFilter(const char* name);
	virtual bool Make(ConfList& conf);
	virtual IProperties* GetProperties(const char* conf) { return m_conf.Get(conf); }
	void SetParentConf(Configurations* parent) { m_conf.SetParent(parent); }
};

class Folder : public IFolder
{
public:
	typedef std::vector<Folder*> Folders;
protected:
	Generator& m_gen;
	Folder::Folders m_folders;
	Project::Projects m_projects;
	File::Files m_files;
	Configurations m_conf;
public:
	Folder(Generator& gen) : m_gen(gen), m_conf(gen) {}
	virtual File* CreateFile(const char* path);
	virtual Project* CreateProject(const char* name);
	virtual Folder* CreateFolder(const char* name);
	virtual bool Make(ConfList& conf);
	virtual IProperties* GetProperties(const char* conf) { return m_conf.Get(conf); }
	void SetParentConf(Configurations* parent) { m_conf.SetParent(parent); }
};

class Solution : public ISolution
{
protected:
	Generator& m_gen;
	Folder::Folders m_folders;
	Project::Projects m_projects;
	File::Files m_files;
	Configurations m_conf;
public:
	Solution(Generator& gen) : m_gen(gen), m_conf(gen) {}
	virtual File* CreateFile(const char* path);
	virtual Project* CreateProject(const char* name);
	virtual Folder* CreateFolder(const char* name);
	virtual bool Make(ConfList& conf);
	virtual IProperties* GetProperties(const char* conf) { return m_conf.Get(conf); }
	void SetParentConf(Configurations* parent) { m_conf.SetParent(parent); }
};



