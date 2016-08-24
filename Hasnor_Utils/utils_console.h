#ifndef HASNOR_CONSOLE_IMPORTED
#define HASNOR_CONSOLE_IMPORTED

#include "utils_types.h"

typedef struct {
	void		(*setVisible)(bool visible);

	void		(*print)(const char* format, ...);
} _console_functions;

_console_functions Console;

#ifdef HASNOR_INIT
void initConsoleFunctions(void);
#endif

#endif
