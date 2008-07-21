// abs.cpp : Defines the entry point for the console application.
//

#include "StdAfx.h"
#include "iout.h"

IManager* CreateGenerator();

class BuildConf : public IBuilder
{
	static int m_genId;
	int m_id;
	ConfList m_conf;
public:
	BuildConf() : m_id(m_genId++) {}
	void ActiveConf(const char* cnf) { m_conf.push_back(cnf); }
	virtual int BuildId() { return m_id; }
	virtual ConfList& GetBuildConf() { return m_conf; }
	virtual IProfile* GetProfile() { return NULL; }
	virtual unsigned long PassDependency() { return ~0; }
};
int BuildConf::m_genId = 1;

class GenTarget : public ITarget
{
	GenTarget* pp;
	int n;
public:
	GenTarget(GenTarget* p=0) : pp(p), n(pp?pp->GetLevel()+1:1) {}

	virtual int GetLevel() { return n; }
	virtual ITarget* GetNext(EObjectType type) { return pp; }
};

#ifdef _WIN32
int _tmain(int argc, _TCHAR* argv[])
#else
int main(int argc, char* argv[])
#endif
{
	IManager* gen = CreateGenerator();
	ISolution* sol = gen->CreateSolution("Hoe");
	IFile* pruv = sol->CreateFile("test.cpp");
	IFolder* tools = sol->CreateFolder("Tools");
	IFile* tf = tools->CreateFolder("Generators")->CreateProject("Hcgen")
		->CreateFilter("Math")->CreateFile("blabla.cpp");
	GenTarget gt;
	GenTarget g2(&gt);
	GenTarget g3(&g2);
	tf->AddTool(&gt, "m4");
	tf->AddTool(&gt, "lex");
	tf->AddTool(&gt, "cc");
	tf->AddTool(&g3, "cc");
	tf->Include(&gt, "debug");
	tf->Include(&gt, "opengl");
	// file muze mit depend nebo src file
	pruv->AddTool(&gt, "doc");
	pruv->AddTool(&g2, "doc");
	tools->AddDepend(&gt, pruv,&g3, false);
	// flex.l flex.cpp flex.o
	// musi se pridat oboji.. 

	IProperties* m = sol->GetProperties();
	sol->GetProperties("release")->SetEnv("build", "release");
	IProperties* d = tools->GetProperties("debug"); 
	IProperties* o = tools->GetProperties("opengl"); 
	m->SetEnv("build", "hoe");
	d->SetEnv("build", "debug");
	o->SetEnv("build", "opengl");	

	// gen configurations...
	{
		BuildConf cnf;
		cnf.ActiveConf("debug");
		cnf.ActiveConf("opengl");
		sol->Make(&cnf,&gt,0);
	}
	return 0;
}


