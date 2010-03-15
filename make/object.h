#pragma once

#include "iout.h"
#include "properties.h"
#include "tool.h"

typedef std::vector<Tool*> Tools;

// pouze dependency wolker
class Manager;

class BuildChecker
{
	int m_buildId;     ///< build id
	int m_level;       ///< aktualni kompilovany level
	int m_targetlevel; ///< cilovy build level
	int m_oklevel;     ///< posledni vybuildeny level bez chyb
	bool m_res;        ///< posledni result
	bool m_build;      ///< probiha build?
public:
	BuildChecker() 
		: m_buildId(-1), m_res(true), 
		m_build(false), m_level(0), m_targetlevel(0), m_oklevel(0) {}
	// check if build?
	bool Build(int level, IBuild* bld) 
	{ 
		int buildId = bld->BuildId();
		if (m_buildId==buildId)
		{
			// assert(!InBuild()) obrana proti zacykleni -> warning -> vede k chybe
			// pri chybe nebuildit dale
			if (!m_res) { m_level = level; return false; } 
			// pokud probiha build nebo je target jiz vybuilden
			if (InBuild() || m_targetlevel >= level)
			{
				// navyseni pozadovaneho buildu
				if (level > m_targetlevel) m_targetlevel = level;
				return false;
			}
		}
		else
		{
			m_buildId=buildId;
			m_oklevel = 0;
			m_level = 0;
			m_targetlevel = 0;
		}
		// vybuildit
		m_build = true;
		// nastavit pozadovany level
		if (level > m_targetlevel) m_targetlevel = level;
		return true;
	}
	// nastavi chybu
	bool Fail() { m_res = false; return false; }
	// getery na levely
	int GetLevel() { return m_level; }
	// v build procesu?
	bool InBuild() { return m_build; }
	// aktualni result
	bool Result() { return m_res; }
	// je hotovy target na levelu?
	bool Result(int level) { return m_oklevel >= level; }

	// uspesny konec buildeni levelu (muze byt i vyssi nez pozadovany)
	bool Done(int level)
	{
		// ukonceni buildu levelu
		assert(level > m_level);
		if (m_res) m_oklevel = level;
		m_level = level;
		// pokud jiz dalsi level neni potreba
		return m_level < m_targetlevel;
	}
	// konec buildovani
	void EndBuild(int level=0) 
	{ 
		// build mohl skoncit i vyssim
		if (level > m_targetlevel) m_targetlevel = level;
		m_build = false; 
		// pokud chyba -> nebuildit dale
		if (!m_res) m_level = m_targetlevel; 
	}
};

class TargetSpec
{
public:
	// selector
	struct Selector
	{
		ConfList include;
		ConfList exclude;
		int buildId;
		bool sel;
	};
	struct Depend
	{
		IObject* obj;
		ITarget* target;
		bool direct;
		Depend(IObject* o, ITarget* t, bool d) : obj(o), target(t),direct(d) {}
	};
	class DepList : public std::vector<Depend>
	{
	public:
		// vybuildi zavislosti
		bool MakeDepend(IBuild* builder, ITool* tool, ITool* link);
	};
	typedef std::vector<Tool*>	ToolList;
	typedef std::map<int, ToolList> ToolMap;
	typedef std::map<int,Selector> SelMap;
	typedef std::map<int,DepList> DepMap;
protected:
	SelMap m_sel;
	// depends
	TargetSpec* m_parent;
	DepMap m_depend;
	// tools
	ToolMap m_tools;
public:
	TargetSpec() : m_parent(NULL) {}
	void AddDepend(int l, IObject* obj, ITarget* target, bool direct) 
	{ m_depend[l].push_back(Depend(obj, target, direct)); }
	bool Select(int l, IBuild*);
	void Include(int l, const char* name);
	void Exclude(int l, const char* name);
	void SetParent(TargetSpec* parent) { m_parent = parent; }
	int GetNextLevel(int l); 
	void AddTool(int l, Tool* tool) { m_tools[l].push_back(tool); }
	ToolList* GetTools(int l) 
	{
		ToolMap::iterator i=m_tools.find(l);
		return i != m_tools.end() ? &i->second:NULL;
	}
	DepList* GetDepends(int l)
	{
		DepMap::iterator i=m_depend.find(l);
		return i != m_depend.end() ? &i->second:NULL;
	}
};

// musim ten object rozdelit :/
struct Level
{
	TargetSpec::ToolList* tools;
	TargetSpec::DepList* deps;
	ITarget* target;
	int level;
};

class Object
{
protected:
	Manager& m_mgr;
	Configurations m_conf;
	Object* m_owner;
	// cache build
	BuildChecker m_check;
	TargetSpec m_targets;

	// make levels
	virtual bool MakeLevel(IBuild* builder, ITool* tool, Level& lvl) = 0;
	bool Build(TargetSpec::ToolList& tools, 
		Properties* prop, IBuild* builder, ITool* out);
public:
	Object(Manager& mgr, Object* owner=NULL) 
		: m_mgr(mgr), m_conf(mgr), m_owner(owner) {}
	void SetOwner(Object* owner) 
	{ 
		m_owner=owner; 
		m_conf.SetParent(&owner->m_conf);
		m_targets.SetParent(&owner->m_targets);
	}
	bool Make(IBuild* builder, ITarget* target, ITool* tool);
	void AddTool(ITarget* target, const char* name);
	virtual void AddTool(ITarget* target, Tool* tool);

	virtual EObjectType GetType() = 0;
};

template<typename IF>
class ObjectRef : public IF, public Object
{
public:
	ObjectRef(Manager& mgr, Object* owner=NULL) 
		: Object(mgr, owner) {}

	virtual IObject* GetOwner() 
		{ return NULL; } //!!!todo
	virtual IProperties* GetProperties(const char* conf) 
		{ return m_conf.Get(conf); }
	virtual void AddDepend(ITarget* target, IObject* depend, ITarget* targetdep, bool direct)
		{ m_targets.AddDepend(target->GetLevel(),depend, targetdep, direct); }
	virtual void Include(ITarget* target, const char* conf) 
		{ m_targets.Include(target->GetLevel(), conf); }
	virtual void Exclude(ITarget* target, const char* conf) 
		{ m_targets.Exclude(target->GetLevel(), conf); }
	virtual void AddTool(ITarget* target, const char* name)
		{ Object::AddTool(target, name); }
	// distribute to child, or set to owner?
	virtual bool Make(IBuild* builder, ITarget* target, ITool* tool)
		{ return Object::Make(builder, target, tool); }
	// forward for type
	virtual EObjectType GetType() { return IF::GetType(); }

};

template<typename LIST, typename ITER> 
class ItemList : public LIST
{
public:
	bool Make(IBuild* builder, ITarget* target, ITool* tool)
	{
		bool ret = true;
		for (ITER i=this->begin();i!=this->end();i++)
			if (!(*i)->Make(builder, target, tool))
				ret = false;
		return ret;
	}
	void AddTool(ITarget* target, Tool* tool)
	{
		for (ITER i=this->begin();i!=this->end();i++)
			static_cast<Object*>(*i)->AddTool(target, tool);
	}
};

class File : public ObjectRef<IFile>
{
public:
	typedef ItemList<std::vector<File*>, std::vector<File*>::iterator> Files;
protected:
	std::string m_path;
	virtual bool MakeLevel(IBuild* builder, ITool* tool, Level& lvl);
public:
	File(Manager& mgr, const char* path, Object* owner=NULL) 
		: ObjectRef<IFile>(mgr, owner), m_path(path) {}
};

class Filter : public ObjectRef<IFilter>
{
public:
	typedef std::vector<Filter*> FilterList;
	typedef ItemList<FilterList, FilterList::iterator> Filters;
protected:
	Filter::Filters m_filters;
	File::Files m_files;
	virtual bool MakeLevel(IBuild* builder, ITool* tool, Level& lvl);
public:
	Filter(Manager& mgr, Object* owner=NULL) : ObjectRef<IFilter>(mgr, owner) {}
	virtual File* CreateFile(const char* path);
	virtual Filter* CreateFilter(const char* name);
	virtual void AddTool(ITarget* target, Tool* tool);
};

class Project : public ObjectRef<IProject>
{
public:
	typedef std::vector<Project*> ProjectList;
	typedef ItemList<ProjectList, ProjectList::iterator> Projects;
protected:
	Filter::Filters m_filters;
	File::Files m_files;
	virtual bool MakeLevel(IBuild* builder, ITool* tool, Level& lvl);
public:
	Project(Manager& mgr, Object* owner=NULL) : ObjectRef<IProject>(mgr, owner) {}
	virtual File* CreateFile(const char* path);
	virtual Filter* CreateFilter(const char* name);
	virtual void AddTool(ITarget* target, Tool* tool);
};

class Folder : public ObjectRef<IFolder>
{
public:
	typedef std::vector<Folder*> FolderList;
	typedef ItemList<FolderList, FolderList::iterator> Folders;
protected:
	Folder::Folders m_folders;
	Project::Projects m_projects;
	File::Files m_files;
	virtual bool MakeLevel(IBuild* builder, ITool* tool, Level& lvl);
public:
	Folder(Manager& mgr, Object* owner=NULL) : ObjectRef<IFolder>(mgr, owner) {}
	virtual File* CreateFile(const char* path);
	virtual Project* CreateProject(const char* name);
	virtual Folder* CreateFolder(const char* name);
	virtual void AddTool(ITarget* target, Tool* tool);
};

class Solution : public ObjectRef<ISolution>
{
protected:
	Folder::Folders m_folders;
	Project::Projects m_projects;
	File::Files m_files;
	virtual bool MakeLevel(IBuild* builder, ITool* tool, Level& lvl);
public:
	Solution(Manager& mgr) : ObjectRef<ISolution>(mgr, NULL) {}
	virtual File* CreateFile(const char* path);
	virtual Project* CreateProject(const char* name);
	virtual Folder* CreateFolder(const char* name);
	virtual void AddTool(ITarget* target, Tool* tool);
};

class Manager : public IOutput
{
public:
	Manager(void);
	~Manager(void);
	// mgr
	virtual File* CreateFile(const char* path);
	virtual Filter* CreateFilter(const char* name);
	virtual Project* CreateProject(const char* name);
	virtual Folder* CreateFolder(const char* name);
	virtual Solution* CreateSolution(const char* name);
	virtual Tool* GetTool(const char* name) = 0;
	virtual Properties* NewProperties();
};


