#include "StdAfx.h"
#include "output.h"
#include "vcgen.h"

IOutput* CreateGenerator()
{
	return new VCGen();
}

Output::Output(void)
{
}

Output::~Output(void)
{
}


