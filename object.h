#pragma once

#include "iout.h"
#include "properties.h"

class Generator;
class Tool;
typedef std::vector<Tool*> Tools;

// pouze dependency wolker

class Object
{
protected:
	Generator& m_gen;
	Configurations m_conf;
	Object* m_owner;
public:
	Object(Generator& gen, Object* owner=NULL) 
		: m_gen(gen), m_conf(gen), m_owner(owner) {}
	void SetOwner(Object* owner) { m_owner=owner; m_conf.SetParent(&owner->m_conf); }
	virtual Object* GetOwner() { return m_owner; }
};

template<typename LIST> 
class ItemList : public LIST
{
public:
	bool Make(ConfList& conf, IBuilder* bld, ITool* tool)
	{
		bool ret = true;
		for (LIST::iterator i=begin();i!=end();i++)
			if (!(*i)->Make(conf, bld, tool))
				ret = false;
		return ret;
	}
};

class File : public Object, public IFile
{
public:
	typedef ItemList<std::vector<File*>> Files;
protected:
	std::string m_path;
	Tools m_tool;
public:
	File(Generator& gen, const char* path, Object* owner=NULL) 
		: Object(gen, owner), m_path(path) {}
	virtual bool Make(ConfList& conf, IBuilder* bld, ITool* tool);
	virtual IProperties* GetProperties(const char* conf) { return m_conf.Get(conf); }
	virtual void AddTool(const char* name);
};

class Filter : public Object, public IFilter
{
public:
	typedef ItemList<std::vector<Filter*>> Filters;
protected:
	Filter::Filters m_filters;
	File::Files m_files;
public:
	Filter(Generator& gen, Object* owner=NULL) : Object(gen, owner) {}
	virtual File* CreateFile(const char* path);
	virtual Filter* CreateFilter(const char* name);
	virtual bool Make(ConfList& conf, IBuilder* bld, ITool* tool);
	virtual IProperties* GetProperties(const char* conf) { return m_conf.Get(conf); }
};

class Project : public Object, public IProject
{
public:
	typedef ItemList<std::vector<Project*>> Projects;
protected:
	Filter::Filters m_filters;
	File::Files m_files;
	Tool* m_link;
public:
	Project(Generator& gen, Object* owner=NULL) : Object(gen, owner), m_link(NULL) {}
	virtual File* CreateFile(const char* path);
	virtual Filter* CreateFilter(const char* name);
	virtual bool Make(ConfList& conf, IBuilder* bld, ITool* tool);
	virtual IProperties* GetProperties(const char* conf) { return m_conf.Get(conf); }
	virtual void SetLinker(const char* name);
};

class Folder : public Object, public IFolder
{
public:
	typedef ItemList<std::vector<Folder*>> Folders;
protected:
	Folder::Folders m_folders;
	Project::Projects m_projects;
	File::Files m_files;
public:
	Folder(Generator& gen, Object* owner=NULL) : Object(gen, owner) {}
	virtual File* CreateFile(const char* path);
	virtual Project* CreateProject(const char* name);
	virtual Folder* CreateFolder(const char* name);
	virtual bool Make(ConfList& conf, IBuilder* bld, ITool* tool);
	virtual IProperties* GetProperties(const char* conf) { return m_conf.Get(conf); }
};

class Solution : public Object, public ISolution
{
protected:
	Folder::Folders m_folders;
	Project::Projects m_projects;
	File::Files m_files;
public:
	Solution(Generator& gen) : Object(gen, NULL) {}
	virtual File* CreateFile(const char* path);
	virtual Project* CreateProject(const char* name);
	virtual Folder* CreateFolder(const char* name);
	virtual bool Make(ConfList& conf, IBuilder* bld, ITool* tool);
	virtual IProperties* GetProperties(const char* conf) { return m_conf.Get(conf); }
};



