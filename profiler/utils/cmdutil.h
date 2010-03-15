

/*
* Copyright
*/

#pragma once
#ifndef _COMMAND_UTIL_H_
#define _COMMAND_UTIL_H_

struct Cmd
{
	const char* name;
	int minargs;
	int maxargs;
	int (*fnc)(int argc, const char* argv[]);
	const char* help;
};

#define DEFINE_COMMANDS Cmd __s_cmd[] = 

#endif // _COMMAND_UTIL_H_

