#include <stdlib.h>
#include <stdio.h>

#define UTIME_INTERVAL 1000000

int
random_interval() {
	unsigned seed = 0;
	time_t t_loc;

	if (time(&t_loc) > 0) {
		seed = (unsigned)t_loc;
		srand(seed);
		return rand_r(&seed);
	} else {
		return -1;
	}
}

int 
random_utime() {

	int r = random_interval();
	if (r < 0) {
		r = UTIME_INTERVAL;
	}
	r = r/1000000;
	return r;
}
