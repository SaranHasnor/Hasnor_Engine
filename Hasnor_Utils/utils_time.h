#ifndef HASNOR_TIME_DEFINED
#define HASNOR_TIME_DEFINED

typedef long timestamp_ms_t;
typedef double timestamp_sec_t;

typedef struct {
	void				(*init)(void);
	void				(*sync)(timestamp_ms_t currentTime);
	timestamp_sec_t		(*seconds)(void);
	timestamp_ms_t		(*milliseconds)(void);
	void				(*beginBenchmark)(const char *name);
	void				(*logBenchmark)(void);
	void				(*endBenchmark)(void);
} _time_functions;

_time_functions Time;

#ifdef HASNOR_INIT
void initTimeFunctions(void);
#endif

#endif