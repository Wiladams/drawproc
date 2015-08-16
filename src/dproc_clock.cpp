#include "dproc_clock.h"

#include <Windows.h>
#include <stdint.h>

// for time keeping
uint64_t startcount = 0;
double clockfrequency = 1;

struct dproc_clock * dproc_clock_new()
{
	struct dproc_clock * clock = (struct dproc_clock *)malloc(sizeof(struct dproc_clock));
	dproc_clock_init(clock);

	return clock;
}

void dproc_clock_free(struct dproc_clock *clock)
{
	free(clock);
}

void dproc_clock_init(struct dproc_clock *clock)
{
	// Get current tick count and frequency
	uint64_t freq;
	::QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	::QueryPerformanceCounter((LARGE_INTEGER*)&clock->startCount);

	clock->frequency = 1.0f / freq;
}

double dproc_clock_seconds(struct dproc_clock *clock)
{
	uint64_t currentCount;
	::QueryPerformanceCounter((LARGE_INTEGER*)&currentCount);

	uint64_t ellapsed = currentCount - clock->startCount;
	double secs = ellapsed * clock->frequency;

	return secs;
}
