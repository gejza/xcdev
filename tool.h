#pragma once

#include "iout.h"

class Builder : public IBuild
{
protected:
	IBuild* m_parent;
public:
	Builder(IBuild* parent=NULL) : m_parent(parent) {}
	virtual int BuildId();
	virtual ConfList& GetBuildConf();
	virtual IProfile* GetProfile();
	virtual unsigned long PassDependency();
	void Set(IBuild* parent) { m_parent = parent; }
};

class Tool : public ITool
{
public:
	Tool(void);
	~Tool(void);

};



