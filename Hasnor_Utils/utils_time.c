#include "utils_time.h"
#include <time.h>

clock_t start;
long offset;

void time_init()
{
	start = clock();
	offset = 0;
}

void time_sync(long currentTime)
{
	offset = currentTime - time_current_ms();
}

double time_current_sec()
{
	clock_t current = clock();

	return ((double)offset / 1000.0) + ((double)(current - start) / CLOCKS_PER_SEC);
}

long time_current_ms()
{
	return offset + (long)(time_current_sec() * 1000);
}