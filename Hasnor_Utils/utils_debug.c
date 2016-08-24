#include "utils_debug.h"
#include "utils_system.h"
#include <assert.h>

void debug_assert(bool expr)
{
	assert(expr);
}

void debug_pause(void)
{
	System.runCommand("pause");
}

void initDebugFunctions(void)
{
	Debug.assert = debug_assert;
	Debug.pause = debug_pause;
}
