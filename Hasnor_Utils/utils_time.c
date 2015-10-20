#include "utils_time.h"
#include <time.h>

clock_t start;
long offset;

void time_init(void)
{
	start = clock();
	offset = 0;
}

double time_current_sec(void)
{
	clock_t current = clock();

	return ((double)offset / 1000.0) + ((double)(current - start) / CLOCKS_PER_SEC);
}

long time_current_ms(void)
{
	return offset + (long)(time_current_sec() * 1000);
}

void time_sync(long currentTime)
{
	offset = currentTime - time_current_ms();
}

void initTimeFunctions(void)
{
	Time.init = time_init;
	Time.sync = time_sync;
	Time.seconds = time_current_sec;
	Time.milliseconds = time_current_ms;
}