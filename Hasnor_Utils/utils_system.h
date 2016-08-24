#ifndef HASNOR_SYSTEM_IMPORTED
#define HASNOR_SYSTEM_IMPORTED

typedef struct {
	int			(*runCommand)(const char *cmd);
	//const char*	(*getVariable)(const char *name);

	void		(*sleep)(unsigned long milliseconds);
} _system_functions;

_system_functions System;

#ifdef HASNOR_INIT
void initSystemFunctions(void);
#endif

#endif
