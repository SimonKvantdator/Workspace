#include <stdio.h>
#include <time.h>
#include <math.h>

int main() {
	int sum = 0;

	struct timespec start_time;
	struct timespec end_time;
    timespec_get(&start_time, TIME_UTC);
	for(int i = 0; i < pow(10, 9); i++) {
		sum += i;
	}
    timespec_get(&end_time, TIME_UTC);
	printf("for-loop took %e [s]\n", end_time.tv_sec + end_time.tv_nsec * 1e-9 - start_time.tv_sec - start_time.tv_nsec * 1e-9);
}
// it takes 2.54e+00 [s] with -O0
// it takes 2.87e-01 [s] with -O1
// it takes 2.12e-07 [s] with -O2
// it takes 3.56e-07 [s] with -O3
// it takes 3.29e-07 [s] with -Os
// it takes 2.89-01 [s] with -Og
// I think some of these optimization algorithms recognize that it's a geometric sum
