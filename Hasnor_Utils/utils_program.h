#ifndef HASNOR_PROGRAM_IMPORTED
#define HASNOR_PROGRAM_IMPORTED

typedef struct {
	void		(*startFunc)(void);

	void		(*updateFunc)(void);
	float		updateRate;

	void		(*shutdownFunc)(void);
} _program_callbacks;

typedef struct {
	int			(*getArguments)(const char ***args);
	const char*	(*getPath)(void);

	void		(*close)(void);

	_program_callbacks	callbacks;
} _program_functions;

_program_functions Program;

extern void HasnorInit(int argc, char** argv);
extern void HasnorRun(void);

#ifdef HASNOR_INIT
void initProgramFunctions(void);
#endif

#endif
