#include <stdint.h>

// Instances of clocks
struct dproc_clock {
	uint64_t startCount;
	double frequency;
};

struct dproc_clock * dproc_clock_new();
void dproc_clock_free(struct dproc_clock *clock);
void dproc_clock_init(struct dproc_clock *clock);
double dproc_clock_seconds(struct dproc_clock *clock); 

//void resettime();
//double seconds();
//void initTime();
