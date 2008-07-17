#include "StdAfx.h"
#include "output.h"
#include "vcgen.h"

IGenerator* CreateGenerator()
{
	return new VCGen();
}

Output::Output(void)
{
}

Output::~Output(void)
{
}


