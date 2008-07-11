#pragma once

#include "iout.h"

class Generator;

struct Item
{
	std::string name;
	std::string value;
};

struct Option
{
	std::string value;
	std::vector<Item> list;
};

typedef std::map<std::string,Option> 

class Properties : public IProperties
{
	Properties* m_parent;

	typedef std::map<std::string,std::string> EnvMap;
	EnvMap m_env;
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
	Generator& m_gen;
	typedef std::map<std::string, Properties*> PropMap;
	PropMap m_optional;
	Properties* m_main;
	Configurations* m_parent;
public:
	Configurations(Generator& gen) 
		: m_gen(gen), m_main(NULL), m_parent(NULL) {}
	Properties* Get(const char* conf=NULL);
	void SetParent(Configurations* parent) { m_parent = parent; }
	// pospojuje vsechny properties podle dulezitosti
	Properties* Serialize(ConfList& conflist);
};


