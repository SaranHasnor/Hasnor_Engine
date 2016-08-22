#include "utils_time.h"
#include <time.h>
#include <windows.h>
#include <stdio.h>

clock_t _start;
long _offset;

LARGE_INTEGER _benchmarkFrequency;
LARGE_INTEGER _benchmarkStart;

const static double CLOCKS_PER_MSEC = CLOCKS_PER_SEC / 1000.0;

void time_init(void)
{
	_start = clock();
	_offset = 0;
}

timestamp_sec_t time_current_sec(void)
{
	clock_t current = clock();

	return ((double)_offset / 1000.0) + ((double)(current - _start) / CLOCKS_PER_SEC);
}

timestamp_ms_t time_current_ms(void)
{
	clock_t current = clock();

	return _offset + (long)((double)(current - _start) / CLOCKS_PER_MSEC);
}

void time_sync(timestamp_ms_t currentTime)
{
	_offset = currentTime - time_current_ms();
}

void _beginBenchmark(const char *name)
{
	QueryPerformanceFrequency(&_benchmarkFrequency);
	QueryPerformanceCounter(&_benchmarkStart);

	printf("===========================\nBenchmark start: %s\n", name);
}

void _logBenchmark(void)
{
	LARGE_INTEGER current;
	QueryPerformanceCounter(&current);

	printf("Benchmark: %fs\n", (current.QuadPart - _benchmarkStart.QuadPart) / _benchmarkFrequency.QuadPart);
}

void _endBenchmark(void)
{
	LARGE_INTEGER end;
	QueryPerformanceCounter(&end);

	printf("End of benchmark: %fs\n===========================\n", (double)(end.QuadPart - _benchmarkStart.QuadPart) / _benchmarkFrequency.QuadPart);
}

void initTimeFunctions(void)
{
	Time.init = time_init;
	Time.sync = time_sync;
	Time.seconds = time_current_sec;
	Time.milliseconds = time_current_ms;
	Time.beginBenchmark = _beginBenchmark;
	Time.logBenchmark = _logBenchmark;
	Time.endBenchmark = _endBenchmark;
}