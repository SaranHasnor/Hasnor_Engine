#ifndef HASNOR_TIME_DEFINED
#define HASNOR_TIME_DEFINED

/*
utils_time

- Simple clock system to keep track of the time elapsed since the beginning of the program
*/

void time_init();
void time_sync(long currentTime);
double time_current_sec();
long time_current_ms();

#endif