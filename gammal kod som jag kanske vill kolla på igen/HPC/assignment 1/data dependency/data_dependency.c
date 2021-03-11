#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define SIZE 1000
#define NBR_BENCHMARK_ITERATIONS 5000

/* 
 * Given an nbr_rows by nbr_cols array matrix, returns the row sums of matrix in row_sums.
 */
void row_sums(double *sums, double **matrix, size_t nbr_rows, size_t nbr_cols) {
	for(int row_index = 0; row_index < nbr_rows; ++row_index) {
		double sum = 0.;
		for(int col_index = 0; col_index < nbr_cols; ++col_index) {
			sum += matrix[row_index][col_index];
		}
		sums[row_index] = sum;
	}
}

void row_sums_unrolled2(double *sums, double **matrix, size_t nbr_rows, size_t nbr_cols) {
	assert(nbr_cols % 2 == 0);

	for(size_t row_index = 0; row_index < nbr_rows; ++row_index) {
		double sum0 = 0.;
		double sum1 = 0.;

		for(size_t col_index = 0; col_index < nbr_cols; col_index += 2) {
			sum0 += matrix[row_index][col_index];
			sum1 += matrix[row_index][col_index+1];
		}
		sums[row_index] = sum0 + sum1;
	}
}

void row_sums_unrolled4(double *sums, double **matrix, size_t nbr_rows, size_t nbr_cols) {
	assert(nbr_cols % 4 == 0);

	for(size_t row_index = 0; row_index < nbr_rows; ++row_index) {
		double sum0 = 0.;
		double sum1 = 0.;
		double sum2 = 0.;
		double sum3 = 0.;

		for(size_t col_index = 0; col_index < nbr_cols; col_index += 4) {
			sum0 += matrix[row_index][col_index];
			sum1 += matrix[row_index][col_index+1];
			sum2 += matrix[row_index][col_index+2];
			sum3 += matrix[row_index][col_index+3];
		}
		sums[row_index] = sum0 + sum1 + sum2 + sum3;
	}
}

void row_sums_unrolled8(double *sums, double **matrix, size_t nbr_rows, size_t nbr_cols) {
	for(size_t row_index = 0; row_index < nbr_rows; ++row_index) {
		double sum0 = 0.;
		double sum1 = 0.;
		double sum2 = 0.;
		double sum3 = 0.;
		double sum4 = 0.;
		double sum5 = 0.;
		double sum6 = 0.;
		double sum7 = 0.;

		for(size_t col_index = 0; col_index < nbr_cols; col_index += 8) {
			sum0 += matrix[row_index][col_index];
			sum1 += matrix[row_index][col_index+1];
			sum2 += matrix[row_index][col_index+2];
			sum3 += matrix[row_index][col_index+3];
			sum4 += matrix[row_index][col_index+4];
			sum5 += matrix[row_index][col_index+5];
			sum6 += matrix[row_index][col_index+6];
			sum7 += matrix[row_index][col_index+7];
		}
		sums[row_index] = sum0 + sum1 + sum2 + sum3 + sum4 + sum5 + sum6 + sum7;
	}
}

void row_sums_unrolled4_reimplemented(double *sums, double **matrix, size_t nbr_rows, size_t nbr_cols) {
	assert(nbr_cols % 4 == 0);

	for(size_t row_index = 0; row_index < nbr_rows; ++row_index) {
		double sum[] = {0., 0., 0., 0.};

		for(size_t col_index = 0; col_index < nbr_cols; col_index += 4) {
			sum[0] += matrix[row_index][col_index];
			sum[1] += matrix[row_index][col_index+1];
			sum[2] += matrix[row_index][col_index+2];
			sum[3] += matrix[row_index][col_index+3];
		}
		sums[row_index] = sum[0] + sum[1] + sum[2] + sum[3];
	}
}

#define BENCHMARK(method) \
	timespec_get(&start_time, TIME_UTC); \
	for (int i = 0; i < NBR_BENCHMARK_ITERATIONS / 10; i++) { \
		method(matrix_row_sums, matrix, SIZE, SIZE); \
	} \
	timespec_get(&end_time, TIME_UTC); \
	\
	printf("Row summing a %d^2 matrix with", SIZE, SIZE); \
	printf("\033[0;36m"); \
	printf(" %s ", #method); /* Print name of method in color */ \
	printf("\033[0m"); \
	printf("took on average %e s\n", (end_time.tv_sec + end_time.tv_nsec * 1e-9 - start_time.tv_sec - start_time.tv_nsec * 1e-9) / ((double) NBR_BENCHMARK_ITERATIONS));

int main() {
	double matrix_row_sums[SIZE];

	struct timespec start_time;
	struct timespec end_time;

	srand((unsigned)time(NULL));

	// Create a SIZE by SIZE matrix and fill it with random entries between -1 and 1
	double **matrix = (double**) malloc(SIZE * sizeof(double*));
	for (int i = 0; i < SIZE; ++i) {
		matrix[i] = (double*) malloc(SIZE * sizeof(double));

		for (int j = 0; j < SIZE; j++) {
			matrix[i][j] = (double) rand() / RAND_MAX * 2. - 1.;
		}
	}

	// Warmup
	for (int i = 0; i < NBR_BENCHMARK_ITERATIONS / 10; i++) {
		row_sums(matrix_row_sums, matrix, SIZE, SIZE);
	}

	BENCHMARK(row_sums);
	BENCHMARK(row_sums_unrolled2);
	BENCHMARK(row_sums_unrolled4);
	BENCHMARK(row_sums_unrolled8);
	BENCHMARK(row_sums_unrolled4_reimplemented);

	// Print a random element of matrix_row_sums and matrix_col_sums
	int index = rand() % SIZE;
	printf("matrix_row_sums[%d] = %f\n", index, matrix_row_sums[index]);
	printf("\n");

	return 0;
}
// compiled with -O0:
// Row summing a 1000^2 matrix with row_sums took on average 						 4.267043e-04 s
// Row summing a 1000^2 matrix with row_sums_unrolled2 took on average 				 3.232546e-04 s
// Row summing a 1000^2 matrix with row_sums_unrolled4 took on average 				 2.702979e-04 s
// Row summing a 1000^2 matrix with row_sums_unrolled8 took on average 				 2.694289e-04 s
// Row summing a 1000^2 matrix with row_sums_unrolled4_reimplemented took on average 3.063479e-04 s
// 
// compiled with -O2:
// Row summing a 1000^2 matrix with row_sums took on average 						 1.832019e-04 s
// Row summing a 1000^2 matrix with row_sums_unrolled2 took on average 				 9.063434e-05 s
// Row summing a 1000^2 matrix with row_sums_unrolled4 took on average 				 5.451307e-05 s
// Row summing a 1000^2 matrix with row_sums_unrolled8 took on average 				 5.280205e-05 s
// Row summing a 1000^2 matrix with row_sums_unrolled4_reimplemented took on average 5.350511e-05 s
// 
// compiled with -O2 - arch=native:
// Row summing a 1000^2 matrix with row_sums took on average 						 1.923637e-04 s
// Row summing a 1000^2 matrix with row_sums_unrolled2 took on average 				 9.052417e-05 s
// Row summing a 1000^2 matrix with row_sums_unrolled4 took on average 				 5.368844e-05 s
// Row summing a 1000^2 matrix with row_sums_unrolled8 took on average 				 5.616732e-05 s
// Row summing a 1000^2 matrix with row_sums_unrolled4_reimplemented took on average 5.598608e-05 s
