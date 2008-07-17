// abs.cpp : Defines the entry point for the console application.
//

#include "StdAfx.h"
#include "iout.h"

IGenerator* CreateGenerator();

#ifdef _WIN32
int _tmain(int argc, _TCHAR* argv[])
#else
int main(int argc, char* argv[])
#endif
{
	IGenerator* gen = CreateGenerator();
	ISolution* sol = gen->CreateSolution("Hoe");
	sol->CreateFile("test.cpp");
	IFolder* tools = sol->CreateFolder("Tools");
	IFile* tf = tools->CreateFolder("Generators")->CreateProject("Hcgen")
		->CreateFilter("Math")->CreateFile("blabla.cpp");
	tf->AddTool("m4");
	tf->AddTool("lex");
	tf->AddTool("cc");
	// file muze mit depend nebo src file

	// flex.l flex.cpp flex.o
	// musi se pridat oboji.. 

	IProperties* m = sol->GetProperties();
	IProperties* d = tools->GetProperties("debug"); 
	IProperties* o = tools->GetProperties("opengl"); 
	m->SetEnv("build", "hoe");
	d->SetEnv("build", "debug");
	o->SetEnv("build", "opengl");	

	// gen configurations...
	ConfList conf;
	conf.push_back("debug");
	conf.push_back("opengl");
	return sol->Make(conf,0,0) ? 0:1;

	// ulozit solutions
}


