
#include <stdio.h>
#include <xc/uuid.hpp>
#include <xc/string.hpp>

// {AE166FCA-91A3-4e1b-938E-9EC6A4D93FAC}
//static const xc::uuid IIDFirewall = 
//{ 0xae166fca, 0x91a3, 0x4e1b, { 0x93, 0x8e, 0x9e, 0xc6, 0xa4, 0xd9, 0x3f, 0xac } };

int main()
{
	XC_VERSION_CHECK;

	xc::uuid id;
	xc::String str;
	str = id.Generate().ToString();
	printf("// {%s}\n", str.Ptr());
	printf("static const xc::uuid id =\n");
	printf("{0x%x, 0x%x, 0x%x, { ", 
		(int)id.data1, (int)id.data2, (int)id.data3, id.data4[0]);
	for (int i=0;i < 8;i++)
	{
		printf(i ? ", 0x%x":"0x%x", (int)id.data4[i]);
	}
	printf(" } };\n");
	return 0;
}