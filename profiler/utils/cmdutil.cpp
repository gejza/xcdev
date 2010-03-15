
#include <stdio.h>
#include <string.h>
#include "cmdutil.h"

extern Cmd __s_cmd[];

const Cmd* find_cmd(const char* cmd)
{
	for (int i=0; __s_cmd[i].name; i++)
	{
		if (strcmp(__s_cmd[i].name, cmd) == 0)
			return __s_cmd + i;
	}
	return NULL;
}

static int help(int argc, const char* argv[])
{
	const Cmd* cmd = find_cmd(argv[0]);
	if (!cmd)
	{
		fprintf(stderr, "Help for command %s not found.\n", argv[0]);
		return 1;
	}
	puts(cmd->help);
	return 0;
}

Cmd c_help = {
	"help", 3, 3, help,
		"help:      usage: help <command>\n"
		"           Print info about command.\n"
};

void usage(const char* exename)
{
	printf("Usage: %s <command> args\n", exename);
	printf("List commands\n");
	for (int i=0; __s_cmd[i].name; i++)
	{
		printf("\n%s", __s_cmd[i].help);
	}
	// print help
	printf("\n"
		"help:      usage: help [command]\n"
		"           Print info about command.\n");
}

int main(int argc, const char* argv[])
{
	if (argc < 2)
	{
		usage(argv[0]);
		return 1;
	}
	const char* exename = argv[0];
	const char* cmdname = argv[1];
	argc -= 2; argv += 2;

	const Cmd* cmd = find_cmd(cmdname);
	if (!cmd) //command not found
	{
		// maybye help
		if (strcmp(cmdname, "help") == 0)
		{
			if (argc)
			{
				for (int i=0;i < argc;i++)
				{
					cmd = find_cmd(argv[i]);
					if (cmd)
					{
						puts(cmd->help);
					}
					else
						fprintf(stderr, "Command %s not found.", argv[i]);
				}
			}
			else
				usage(exename);
			return 0;
		}
		else
		{
			fprintf(stderr, "Command %s not found.\n", cmdname);
			usage(exename);
			return 1;
		}
	}

	//check arguments
	if (argc < cmd->minargs)
	{
		fprintf(stderr, "Command %s requie %d arguments.\n%s",
			cmdname, cmd->minargs, cmd->help);
		return 1;
	}
	if (cmd->maxargs > 0 && argc > cmd->maxargs)
	{
		fprintf(stderr, "%d is too match arguments for command %s.\n%s",
			cmd->maxargs, cmdname, cmd->help);
		return 1;
	}
	return cmd->fnc(argc, argv);
}

