#ifndef HASNOR_TIME_DEFINED
#define HASNOR_TIME_DEFINED

/*
utils_time

- Simple clock system to keep track of the time elapsed since the beginning of the program
*/

typedef struct {
	void (*init)(void);
	void (*sync)(long currentTime);
	double (*seconds)(void);
	long (*milliseconds)(void);
} _time_functions;

_time_functions Time;

#ifdef HASNOR_INIT
void initTimeFunctions(void);
#endif

#endif