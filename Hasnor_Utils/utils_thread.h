#ifndef HASNOR_THREAD_DEFINED
#define HASNOR_THREAD_DEFINED

#include "utils_types.h"

typedef struct {
	uint		id;
	bool		(*function)(void);

	bool		loops;
	uint		delayBetweenLoops;

	bool		destroyWhenDone;
} thread_t;

typedef struct {
	uint		(*newMutex)(const char *name);
	thread_t*	(*newThread)(void);

	void		(*deleteMutex)(uint id);

	void		(*run)(thread_t *thread);

	void		(*lockMutex)(uint id);
	void		(*unlockMutex)(uint id);
} _thread_functions;

_thread_functions Thread;

#ifdef HASNOR_INIT
void initThreadFunctions(void);
#endif

#endif
