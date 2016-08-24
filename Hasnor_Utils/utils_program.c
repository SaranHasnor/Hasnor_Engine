#include "utils_program.h"
#include "utils.h"
#include "utils_string.h"
#include "utils_time.h"
#include "utils_system.h"
#include <stdlib.h>

#include "utils_init.h"

static char *_path = NULL;
static char **_argv = NULL;
static int _argc = 0;

void program_init(int argc, char** argv)
{
	int i;

	_path = String.create(argv[0]);
	
	_argc = argc - 1;

	if (_argc > 0)
	{
		_argv = allocArray(char*, _argc);
		for (i = 1; i < argc; i++)
		{
			_argv[i - 1] = String.create(argv[i]);
		}
	}
	else
	{
		_argv = NULL;
	}
}

int program_getArguments(const char ***args)
{
	*args = _argv;
	return _argc;
}

const char* program_getPath(void)
{
	return _path;
}

void program_close(void)
{
	if (Program.callbacks.shutdownFunc)
	{
		Program.callbacks.shutdownFunc();
	}
	Memory.freeAll();
	exit(0);
}

void initProgramFunctions(void)
{
	Program.getArguments = program_getArguments;
	Program.getPath = program_getPath;

	Program.close = program_close;

	Program.callbacks.startFunc = NULL;
	Program.callbacks.updateFunc = NULL;
	Program.callbacks.updateRate = 0.0f;
	Program.callbacks.shutdownFunc = NULL;
}

void HasnorInit(int argc, char** argv)
{
	initHasnorUtils();

	program_init(argc, argv);

	Time.init();
}

void HasnorRun(void)
{
	if (Program.callbacks.startFunc)
	{
		Program.callbacks.startFunc();
	}

	if (Program.callbacks.updateFunc)
	{
		unsigned long updateTimer = 0;
		if (Program.callbacks.updateRate > 0.0f && Program.callbacks.updateRate < 1000.0f)
		{
			updateTimer = (unsigned long)(1000.0f / Program.callbacks.updateRate);
		}

		while (true)
		{
			Program.callbacks.updateFunc();
				
			System.sleep(updateTimer);
		}
	}
	else if (Program.callbacks.shutdownFunc)
	{
		Program.callbacks.shutdownFunc();
	}
}
