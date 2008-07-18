#include "StdAfx.h"
#include "output.h"
#include "vcgen.h"

IManager* CreateGenerator()
{
	return new VCGen();
}

Output::Output(void)
{
}

Output::~Output(void)
{
}


