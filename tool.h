#pragma once

#include "iout.h"

class Builder : public IBuilder
{
protected:
	IBuilder* m_parent;
public:
	Builder(IBuilder* parent=NULL) : m_parent(parent) {}
	virtual int BuildId();
	virtual ConfList& GetBuildConf();
	virtual IProfile* GetProfile();
	virtual unsigned long PassDependency();
	void Set(IBuilder* parent) { m_parent = parent; }
};

class Tool : public ITool
{
public:
	Tool(void);
	~Tool(void);

};



