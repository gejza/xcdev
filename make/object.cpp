#include "StdAfx.h"
#include "object.h"
#include "generator.h"

///////////////////////////
// Selector
bool TargetSpec::Select(int l, IBuild* builder)
{
	// neni co na praci...
	if (m_sel.find(l) == m_sel.end())
	{
		//m_sel[l].buildId = buildId;
		//m_sel[l].sel = (m_parent) ? m_parent->Select(l, builder) : true;
		//return sel;
		return (m_parent) ? m_parent->Select(l, builder) : true;
	}
	int buildId = builder->BuildId();
	if (m_sel[l].buildId == buildId) return m_sel[l].sel;
	m_sel[l].buildId = buildId;
	// include?
	ConfList& cfl = builder->GetBuildConf();
	// find include
	bool sel = (m_parent) ? m_parent->Select(l, builder) : true;
	// find exclude
	ConfList& exclude = m_sel[l].exclude;
	ConfList& include = m_sel[l].include;
	for (ConfList::iterator i=exclude.begin();sel && i != exclude.end();i++)
		for (ConfList::iterator j=cfl.begin();sel && j != cfl.end();j++)
			if (*i == *j)
				sel = false; // vyhozeni z buildu
	// nastavene musi byt vsechny
	for (ConfList::iterator i=include.begin();sel && i != include.end();i++)
	{
		sel = false;
		for (ConfList::iterator j=cfl.begin();!sel && j != cfl.end();j++)
			if (*i == *j)
				sel = true; // vraceni do buildu
	}
	return m_sel[l].sel = sel;
}

void TargetSpec::Include(int l, const char* name)
{
	m_sel[l].include.push_back(name);
}

void TargetSpec::Exclude(int l, const char* name)
{
	m_sel[l].exclude.push_back(name);
}

template<typename MAP, typename ITER>
int GetNext(MAP& map, int l)
{
	for (ITER i=map.begin();
		i!=map.end();i++)
	{
		if (i->first > l)
			return i->first;
	}
	return 0;
}

int TargetSpec::GetNextLevel(int l)
{
	int lt = GetNext<ToolMap, ToolMap::iterator>(m_tools, l);
	int ld = GetNext<DepMap,DepMap::iterator>(m_depend, l);
	if (lt && !ld) return lt;
	if (!lt && ld) return ld;
	return lt < ld ? lt:ld;
}

// depend
bool TargetSpec::DepList::MakeDepend(IBuild* builder, ITool* tool, ITool* link)
{
	// omezeni na zavislost
	unsigned long pass = builder->PassDependency();
	bool ret = true;
	for (DepList::iterator i=this->begin();i != this->end();i++)
	{
		if (i->obj->GetType() & pass) // can build depend
		{
			if (!i->obj->Make(builder, i->target, i->direct ? link:tool))
				ret = false;
		}
	}
	return ret;
}

bool Object::Make(IBuild* builder, ITarget* target, ITool* tool)
{
	// probehnout pro kazdy target?
	// clean and install
	int tlevel = target->GetLevel();
	if (!m_check.Build(tlevel, builder)) 
		return m_check.Result(tlevel); // vysledek tazaneho buildu

	int level = m_check.GetLevel();
	bool tbuild = false;
	// vybuildit 
	Level lvl;
	lvl.target = target;
	while ((level = m_targets.GetNextLevel(level)) || !tbuild)
	{
		// don't skip target level
		if (!tbuild && (!level || level > tlevel)) level = tlevel;
		if (level == tlevel) tbuild = true;
		// nizzsi buildy nemaji vystup
		// selector and make
		lvl.tools = m_targets.GetTools(level);
		lvl.deps = m_targets.GetDepends(level);
		lvl.level = level;
		if (m_targets.Select(level, builder) && 
			!MakeLevel(builder, tlevel==level ? tool:NULL, lvl))
		{
			m_check.Fail(); // chyba v buildeni -> konec
			m_check.EndBuild(level);
			return m_check.Result(tlevel);
		}
		// vyzadovat buildeni jen do ciloveho buildu
		if (!m_check.Done(level))
			break;
	}
	// vse by melo byt ok => zkusit buildit rovnou dalsi targets
	while (target = target->GetNext(this->GetType()))
	{
		level = target->GetLevel();
		if (level <= m_check.GetLevel()) continue;
		lvl.tools = m_targets.GetTools(level);
		lvl.deps = m_targets.GetDepends(level);
		lvl.level = level;
		lvl.target = target;
		if (m_targets.Select(level, builder) && 
			!MakeLevel(builder, NULL, lvl)) // vyssi buildy pouze jen do builderu
		{
			// oznaceni buildu jako neplatneho, odejit nebo jeste zkusit?
			m_check.Fail();
			break;
		}
		m_check.Done(level);
	}
	m_check.EndBuild(level);
	return m_check.Result(tlevel);
}

bool Object::Build(TargetSpec::ToolList& tools, Properties* prop, 
				   IBuild* builder, ITool* out)
{
	TargetSpec::ToolList::iterator t=tools.begin();
	while (t != tools.end())
	{
		ITool* tool = (*t);
		t++;
		ITool* o = (t == tools.end()) ? out:(*t);
		if (!tool->Build(prop, builder, o))
		{
			m_check.Fail();
			return false;
		}
	}
	return true;
}

void Object::AddTool(ITarget* target, const char* name)
{
	AddTool(target, m_mgr.GetTool(name));
}

void Object::AddTool(ITarget *target, Tool *tool)
{
	EObjectType tt = tool->ObjectUsed();
	EObjectType ot = this->GetType();
	if (tt == ot)
		m_targets.AddTool(target->GetLevel(), tool);
	else if (tt > ot && m_owner)
		m_owner->AddTool(target, tool);
	else
		throw; // todo warning
}

/////////////////////////
// file
bool File::MakeLevel(IBuild* builder, ITool* tool, Level& level)
{
	if (!level.tools || level.tools->empty()) // no tools for build in this level
	{
		if (level.deps && !level.deps->MakeDepend(builder, tool, tool))
			m_check.Fail();
		return true;
	}
	Properties* prop = m_conf.Serialize(builder->GetBuildConf(), builder->BuildId());

	// init first tool
	level.tools->front()->AddSource(m_path.c_str());
	if (level.deps && !level.deps->MakeDepend(builder, tool, level.tools->front()))
		m_check.Fail();

	// link tools ?
	if (!Build(*level.tools, prop, builder, tool))
		m_check.Fail();

	return m_check.Result();
}

/////////////////////////
// filter
bool Filter::MakeLevel(IBuild* builder, ITool* tool, Level& level)
{
	// make folders, make projects -> podle dependency!!!
	if (level.deps && !level.deps->MakeDepend(builder, tool, tool))
		m_check.Fail();

	if (!m_filters.Make(builder, level.target, tool))
		m_check.Fail();
	if (!m_files.Make(builder, level.target, tool))
		m_check.Fail();
	return m_check.Result();
}

void Filter::AddTool(ITarget *target, Tool *tool)
{
	EObjectType tt = tool->ObjectUsed();
	EObjectType ot = this->GetType();
	if (ot > tt)
	{
		m_filters.AddTool(target, tool);
		m_files.AddTool(target, tool);
	}
	else
		Object::AddTool(target, tool);
}

File* Filter::CreateFile(const char* path)
{
	m_files.push_back(m_mgr.CreateFile(path));
	m_files.back()->SetOwner(this);
	return m_files.back();
}

Filter* Filter::CreateFilter(const char* name)
{
	m_filters.push_back(m_mgr.CreateFilter(name));
	m_filters.back()->SetOwner(this);
	return m_filters.back();
}

/////////////////////////
// Project
bool Project::MakeLevel(IBuild* builder, ITool* tool, Level& level)
{
	// make folders, make projects -> podle dependency!!!
	// get link
	ITool* t = level.tools && !level.tools->empty() ?
		level.tools->front():tool;

	if (level.deps && !level.deps->MakeDepend(builder, tool, t))
		m_check.Fail();

	if (!m_filters.Make(builder, level.target, t))
		m_check.Fail();
	if (!m_files.Make(builder, level.target, t))
		m_check.Fail();
	// link
	if (m_check.Result() && t != tool) // link
	{
			// link tools ?
		if (!Build(*level.tools, 
					m_conf.Serialize(builder->GetBuildConf(), builder->BuildId()),
					builder, tool))
			m_check.Fail();
	}
	return m_check.Result();
}

void Project::AddTool(ITarget *target, Tool *tool)
{
	EObjectType tt = tool->ObjectUsed();
	EObjectType ot = this->GetType();
	if (ot > tt)
	{
		m_filters.AddTool(target, tool);
		m_files.AddTool(target, tool);
	}
	else
		Object::AddTool(target, tool);
}

File* Project::CreateFile(const char* path)
{
	m_files.push_back(m_mgr.CreateFile(path));
	m_files.back()->SetOwner(this);
	return m_files.back();
}

Filter* Project::CreateFilter(const char* name)
{
	m_filters.push_back(m_mgr.CreateFilter(name));
	m_filters.back()->SetOwner(this);
	return m_filters.back();
}

/////////////////////////
// folder
bool Folder::MakeLevel(IBuild* builder, ITool* tool, Level& level)
{
	// make folders, make projects -> podle dependency!!!
	if (level.deps && !level.deps->MakeDepend(builder, tool, tool))
		m_check.Fail();

	if (!m_folders.Make(builder, level.target, tool))
			m_check.Fail();
	if (!m_projects.Make(builder, level.target, tool))
			m_check.Fail();
	return m_check.Result();
}

void Folder::AddTool(ITarget *target, Tool *tool)
{
	EObjectType tt = tool->ObjectUsed();
	EObjectType ot = this->GetType();
	if (ot > tt)
	{
		m_folders.AddTool(target, tool);
		m_projects.AddTool(target, tool);
	}
	else
		Object::AddTool(target, tool);
}

File* Folder::CreateFile(const char* path)
{
	m_files.push_back(m_mgr.CreateFile(path));
	m_files.back()->SetOwner(this);
	return m_files.back();
}

Project* Folder::CreateProject(const char* name)
{
	m_projects.push_back(m_mgr.CreateProject(name));
	m_projects.back()->SetOwner(this);
	return m_projects.back();
}

Folder* Folder::CreateFolder(const char* name)
{
	m_folders.push_back(m_mgr.CreateFolder(name));
	m_folders.back()->SetOwner(this);
	return m_folders.back();
}

/////////////////////////
// solution
File* Solution::CreateFile(const char* path)
{
	m_files.push_back(m_mgr.CreateFile(path));
	m_files.back()->SetOwner(this);
	return m_files.back();
}

Project* Solution::CreateProject(const char* name)
{
	m_projects.push_back(m_mgr.CreateProject(name));
	m_projects.back()->SetOwner(this);
	return m_projects.back();
}

Folder* Solution::CreateFolder(const char* name)
{
	m_folders.push_back(m_mgr.CreateFolder(name));
	m_folders.back()->SetOwner(this);
	return m_folders.back();
}

bool Solution::MakeLevel(IBuild* builder, ITool* tool, Level& level)
{
	if (!m_folders.Make(builder, level.target, tool))
		m_check.Fail();
	if (!m_projects.Make(builder, level.target, tool))
		m_check.Fail();
	if (!m_files.Make(builder, level.target, tool))
		m_check.Fail();

	return m_check.Result();
}

void Solution::AddTool(ITarget *target, Tool *tool)
{
	EObjectType tt = tool->ObjectUsed();
	EObjectType ot = this->GetType();
	if (ot > tt)
	{
		m_folders.AddTool(target, tool);
		m_projects.AddTool(target, tool);
		m_files.AddTool(target, tool);
	}
	else
		Object::AddTool(target, tool);
}

////////////////////////////////////////////////////
// Manager
Manager::Manager(void)
{
}

Manager::~Manager(void)
{
}

File* Manager::CreateFile(const char* path)
{
	return new File(*this, path);
}

Filter* Manager::CreateFilter(const char* name)
{
	return new Filter(*this);
}

Project* Manager::CreateProject(const char* name)
{
	return new Project(*this);
}

Folder* Manager::CreateFolder(const char* name)
{
	return new Folder(*this);
}

Solution* Manager::CreateSolution(const char* name)
{
	// vytvorit novou solution
	// solution jako folder?

	return new Solution(*this);
}

Properties* Manager::NewProperties()
{
	return new Properties();
}

