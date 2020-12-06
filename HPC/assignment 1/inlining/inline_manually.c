#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "constants.h"

int main() {
	// Declare three ARRAY_LENGTH-vectors of complex numbers
	double as_re[ARRAY_LENGTH];
	double as_im[ARRAY_LENGTH];
	double bs_re[ARRAY_LENGTH];
	double bs_im[ARRAY_LENGTH];
	double cs_re[ARRAY_LENGTH];
	double cs_im[ARRAY_LENGTH];

	struct timespec start_time;
	struct timespec end_time;

	srand((unsigned)time(NULL));

	// Warmup
	for (int i = 0; i < NBR_BENCHMARK_ITERATIONS / 10; i++) {
		// Initiate the b and c vectors in the "unit square"
		for (int j = 0; j < ARRAY_LENGTH; j++) {
			bs_re[j] = (double) rand() / RAND_MAX * 2. - 1.;
			bs_im[j] = (double) rand() / RAND_MAX * 2. - 1.;
			cs_re[j] = (double) rand() / RAND_MAX * 2. - 1.;
			cs_im[j] = (double) rand() / RAND_MAX * 2. - 1.;
		}

		// Multiply the b and c vectors
		for (int j = 0; j < ARRAY_LENGTH; j++) {
			as_re[j] = bs_re[j] * cs_re[j] - bs_im[j] * cs_im[j];
			as_im[j] = bs_re[j] * cs_im[j] + bs_im[j] * cs_re[j];
		}
	}

	// Benchmarked run
	timespec_get(&start_time, TIME_UTC);
	for (int i = 0; i < NBR_BENCHMARK_ITERATIONS; i++) {
		// Initiate the b and c vectors in the "unit square"
		for (int j = 0; j < ARRAY_LENGTH; j++) {
			bs_re[j] = (double) rand() / RAND_MAX * 2. - 1.;
			bs_im[j] = (double) rand() / RAND_MAX * 2. - 1.;
			cs_re[j] = (double) rand() / RAND_MAX * 2. - 1.;
			cs_im[j] = (double) rand() / RAND_MAX * 2. - 1.;
		}

		// Multiply the b and c vectors
		for (int j = 0; j < ARRAY_LENGTH; j++) {
			as_re[j] = bs_re[j] * cs_re[j] - bs_im[j] * cs_im[j];
			as_im[j] = bs_re[j] * cs_im[j] + bs_im[j] * cs_re[j];
		}
	}

	// Print benchmark time
	timespec_get(&end_time, TIME_UTC);
	printf("Multiplying %d complex numbers took an average of %e s\n", ARRAY_LENGTH, (end_time.tv_sec + end_time.tv_nsec * 1e-9 - start_time.tv_sec - start_time.tv_nsec * 1e-9) / ((double) NBR_BENCHMARK_ITERATIONS));

	// Print a random element of as
	int index = rand() % ARRAY_LENGTH;
	printf("a[%d] = %f + %fi\n", index, as_re[index], as_im[index]);

	return 0;
}

/*Given pointers to a_re, a_im, b_re, b_im, c_re, and c_im, returns the product of the complex numbers b and c in a*/
void mul_cpx(double* a_re, double* a_im, double* b_re, double* b_im, double* c_re, double* c_im) {
}
// Multiplying 30000 complex numbers took an average (over 1000000 benchmark iterations) of 2.017295e-03 s
