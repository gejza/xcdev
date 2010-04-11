
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "xc/debug.h"

namespace {
	bool g_debug = false;
}

void xc::debug::EnableDebug(bool enable)
{
	g_debug = enable;
}

void xc::debug::Print(int type, const char* format, ...)
{
	if (!g_debug)
		return;
	va_list arg;
	va_start(arg, format);
	xc::debug::PrintV(type, format, arg);
	va_end(arg);
}

void xc::debug::PrintV(int type, const char* format, va_list arg)
{
	if (!g_debug)
		return;
	printf("STRING ");
	vprintf(format, arg);
	printf("\n");
}

void xc::debug::Assert(bool exp, const char* file, int line, const char* format, ...)
{
	if (exp)
		return;
	va_list arg;
	va_start(arg, format);
	fprintf(stderr, "ASSERT: ");
	vfprintf(stderr, format, arg);
	fprintf(stderr, " on %s(%d)\n", file, line);
	va_end(arg);
	exit(1);
}
