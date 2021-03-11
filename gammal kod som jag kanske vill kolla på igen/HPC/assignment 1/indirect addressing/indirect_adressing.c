#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define ARRAY_SIZE 1000000
#define STEP_SIZE 1000
#define NBR_BENCHMARK_ITERATIONS 1000


int main() {
	double *x = malloc(ARRAY_SIZE * sizeof(double));
	double *y = malloc(ARRAY_SIZE * sizeof(double));
	double a = 1.2345;

	struct timespec start_time;
	struct timespec end_time;

	// Linearly initialize p
	double *p = malloc(ARRAY_SIZE * sizeof(double));
	for(size_t i = 0; i < ARRAY_SIZE; ++i) {
		p[i] = i;
	}

	// Benchmark
	timespec_get(&start_time, TIME_UTC);
	for(size_t h = 0; h < NBR_BENCHMARK_ITERATIONS; h++) {
		for(size_t i = 0; i < ARRAY_SIZE; ++i) {
			size_t j = p[i];
			y[j] += a * x[j];
		}
	}
	timespec_get(&end_time, TIME_UTC);
	printf("Addition with linearly initialized p took on average %e s\n", (end_time.tv_sec + end_time.tv_nsec * 1e-9 - start_time.tv_sec - start_time.tv_nsec * 1e-9) / ((double) NBR_BENCHMARK_ITERATIONS));


	// Initialize p in steps of STEP_SIZE
	assert(ARRAY_SIZE % STEP_SIZE == 0);
	for(size_t j = 0, k = 0; j < STEP_SIZE; ++j) {
		for (size_t i = j; i < ARRAY_SIZE; i += STEP_SIZE, ++k) {
			p[i] = k;
		}
	}

	// Benchmark
	y = malloc(ARRAY_SIZE * sizeof(double));
	timespec_get(&start_time, TIME_UTC);
	for(size_t h = 0; h < NBR_BENCHMARK_ITERATIONS; h++) {
		for(size_t i = 0; i < ARRAY_SIZE; ++i) {
			size_t j = p[i];
			y[j] += a * x[j];
		}
	}
	timespec_get(&end_time, TIME_UTC);
	printf("Addition with initialized in steps of %d p took on average %e s\n", STEP_SIZE, (end_time.tv_sec + end_time.tv_nsec * 1e-9 - start_time.tv_sec - start_time.tv_nsec * 1e-9) / ((double) NBR_BENCHMARK_ITERATIONS));

	// Benchmark without p
	y = malloc(ARRAY_SIZE * sizeof(double));
	timespec_get(&start_time, TIME_UTC);
	for(size_t h = 0; h < NBR_BENCHMARK_ITERATIONS; h++) {
		for(size_t i = 0; i < ARRAY_SIZE; ++i) {
			y[i] += a * x[i];
		}
	}
	timespec_get(&end_time, TIME_UTC);
	printf("Addition without p took on average %e s\n", (end_time.tv_sec + end_time.tv_nsec * 1e-9 - start_time.tv_sec - start_time.tv_nsec * 1e-9) / ((double) NBR_BENCHMARK_ITERATIONS));
	
	printf("\n");
	return 0;
}
// When compiled with -O0:
// Addition with linearly initialized p took on average 9.887316e-03 s
// Addition with initialized in steps of 1000 p took on average 1.708833e-02 s
// Addition without p took on average 4.787759e-03 s
// 
// When compiled with -O2:
// Addition with linearly initialized p took on average 2.624191e-03 s
// Addition with initialized in steps of 1000 p took on average 1.232323e-02 s
// Addition without p took on average 1.845049e-03 s
// 
// When compiled with -O2 -march=native:
// Addition with linearly initialized p took on average 1.821385e-03 s
// Addition with initialized in steps of 1000 p took on average 1.191763e-02 s
// Addition without p took on average 1.157117e-03 s
