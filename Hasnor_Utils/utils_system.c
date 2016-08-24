#include "utils_system.h"
#include <Windows.h>

int system_runCommand(const char *cmd)
{
	return system(cmd);
}

void system_sleep(unsigned long milliseconds)
{
	Sleep(milliseconds);
}

void initSystemFunctions(void)
{
	System.runCommand = system_runCommand;
	System.sleep = system_sleep;
}
