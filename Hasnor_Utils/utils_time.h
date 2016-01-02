#ifndef HASNOR_TIME_DEFINED
#define HASNOR_TIME_DEFINED

typedef struct {
	void (*init)(void);
	void (*sync)(long currentTime);
	double (*seconds)(void);
	long (*milliseconds)(void);
	void (*beginBenchmark)(const char *name);
	void (*logBenchmark)(void);
	void (*endBenchmark)(void);
} _time_functions;

_time_functions Time;

#ifdef HASNOR_INIT
void initTimeFunctions(void);
#endif

#endif