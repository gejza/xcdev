#pragma once

#include "iout.h"

class Manager;

struct Define
{
	std::string name;
	std::string value;
};

struct DefineList
{
	std::vector<Define> list;
	bool inherit_parent;
};

struct ValueList
{
	std::vector<std::string> list;
	bool inherit_parent;
};

struct Option
{
	enum Type
	{
		OTBool,
		OTString,
		OTDefines,
		OTValues,
	} type;
	union {
		bool b;
		const char* str;
		DefineList* defines;
		ValueList* values;
	} value;
};

typedef std::map<std::string, Option> Section;
typedef std::map<std::string, Section> Sections;

class Properties : public IProperties
{
	Properties* m_parent;

	typedef std::map<std::string,std::string> EnvMap;
	EnvMap m_env;
	Sections m_sec;
public:
	Properties(void);
	~Properties(void);

	// seters
	virtual void Set(const char* section, const char* prop, const char* str);
	// geters
	virtual const char* GetString(const char* section, const char* prop);
	// others
	virtual void Remove(const char* section, const char* prop);
	virtual const char* GetEnv(const char* name);
	virtual void SetEnv(const char* name, const char* value);
	virtual bool IsSelect(); //< selector pro skupiny
	virtual void SetParent(Properties* prop); //< nastavi zdedene properties
};

class Configurations
{
	Manager& m_mgr;
	typedef std::map<std::string, Properties*> PropMap;
	PropMap m_optional;
	Properties* m_main;
	Configurations* m_parent;
	// cache
	int m_buildId;
	Properties* m_serialized;
public:
	Configurations(Manager& mgr) 
		: m_mgr(mgr), m_main(NULL), m_parent(NULL), m_buildId(0) {}
	Properties* Get(const char* conf=NULL);
	void SetParent(Configurations* parent) { m_parent = parent; }
	// pospojuje vsechny properties podle dulezitosti
	Properties* Serialize(ConfList& conflist, int buildId);
};


