#ifndef HASNOR_DEBUG_IMPORTED
#define HASNOR_DEBUG_IMPORTED

#include "utils_types.h"

typedef struct {
	void		(*assert)(bool expr);
	void		(*pause)(void);
} _debug_functions;

_debug_functions Debug;

#ifdef HASNOR_INIT
void initDebugFunctions(void);
#endif

#endif
