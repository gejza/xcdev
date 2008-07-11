#include "StdAfx.h"
#include "output.h"

IGenerator* CreateGenerator()
{
	return new Output();
}

Output::Output(void)
{
}

Output::~Output(void)
{
}


