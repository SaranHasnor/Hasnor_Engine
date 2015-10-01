#ifndef HASNOR_TIME_DEFINED
#define HASNOR_TIME_DEFINED

/*
utils_time

- Simple clock system to keep track of the time elapsed since the beginning of the program
*/

typedef struct {
	void (*init)();
	void (*sync)(long currentTime);
	double (*seconds)();
	long (*milliseconds)();
} _time_functions;

_time_functions Time;

void initTimeFunctions();

#endif