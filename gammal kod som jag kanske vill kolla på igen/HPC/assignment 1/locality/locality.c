#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define SIZE 1000
#define NBR_BENCHMARK_ITERATIONS 5000

/* 
 * Given an nbr_rows by nbr_cols array matrix, returns the row sums of matrix in row_sums.
 */
void row_sums(double *sums, double **matrix, int nbr_rows, int nbr_cols) {
	double sum;

	for(int row_index = 0; row_index < nbr_rows; ++row_index) {
		sum = 0.;
		for(int col_index = 0; col_index < nbr_cols; ++col_index) {
			sum += matrix[row_index][col_index];
		}
		sums[row_index] = sum;
	}
}

/* 
 * Given an nbr_rows by nbr_cols array matrix, returns the column sums of matrix in col_sums.
 */
void col_sums(double *sums, double **matrix, int nbr_rows, int nbr_cols) {
	double sum;

	for(int col_index = 0; col_index < nbr_cols; ++col_index) {
		sum = 0.;
		for(int row_index = 0; row_index < nbr_rows; ++row_index) {
			sum += matrix[row_index][col_index];
		}
		sums[col_index] = sum;
	}
}

/* 
 * Given an nbr_rows by nbr_cols array matrix, returns the column sums of matrix in col_sums, but more utilizing memory locality.
 */
void improved_col_sums(double *sums, double **matrix, int nbr_rows, int nbr_cols) {
	// Initialize to 0?
	for (int i = 0; i < nbr_cols; i++) {
		sums[i] = 0.;
	}

	for(int row_index = 0; row_index < nbr_rows; ++row_index) {
		for(int col_index = 0; col_index < nbr_cols; ++col_index) {
			sums[col_index] += matrix[row_index][col_index];
		}
	}
}


int main() {
	double matrix_row_sums[SIZE];
	double matrix_col_sums[SIZE];

	struct timespec start_time;
	struct timespec end_time;

	srand((unsigned)time(NULL));

	// Create matrix and fill it with random entries between -1 and 1
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
		col_sums(matrix_col_sums, matrix, SIZE, SIZE);
	}


	// Benchmarked row sum run
	timespec_get(&start_time, TIME_UTC);
	for (int i = 0; i < NBR_BENCHMARK_ITERATIONS / 10; i++) {
		row_sums(matrix_row_sums, matrix, SIZE, SIZE);
	}
	timespec_get(&end_time, TIME_UTC);

	// Print benchmark time
	printf("Taking a row sum of an %d by %d matrix took an average of %e s\n", SIZE, SIZE, (end_time.tv_sec + end_time.tv_nsec * 1e-9 - start_time.tv_sec - start_time.tv_nsec * 1e-9) / ((double) NBR_BENCHMARK_ITERATIONS));


	// Benchmarked column sum run
	timespec_get(&start_time, TIME_UTC);
	for (int i = 0; i < NBR_BENCHMARK_ITERATIONS / 10; i++) {
		col_sums(matrix_col_sums, matrix, SIZE, SIZE);
	}
	timespec_get(&end_time, TIME_UTC);

	// Print benchmark time
	printf("Taking a column sum of an %d by %d matrix took an average of %e s\n", SIZE, SIZE, (end_time.tv_sec + end_time.tv_nsec * 1e-9 - start_time.tv_sec - start_time.tv_nsec * 1e-9) / ((double) NBR_BENCHMARK_ITERATIONS));


	// Benchmarked improved column sum run
	timespec_get(&start_time, TIME_UTC);
	for (int i = 0; i < NBR_BENCHMARK_ITERATIONS / 10; i++) {
		improved_col_sums(matrix_col_sums, matrix, SIZE, SIZE);
	}
	timespec_get(&end_time, TIME_UTC);

	// Print benchmark time
	printf("Taking (improved) column sum of an %d by %d matrix took an average of %e s\n", SIZE, SIZE, (end_time.tv_sec + end_time.tv_nsec * 1e-9 - start_time.tv_sec - start_time.tv_nsec * 1e-9) / ((double) NBR_BENCHMARK_ITERATIONS));


	// Print a random element of matrix_row_sums and matrix_col_sums
	int index = rand() % SIZE;
	printf("matrix_row_sums[%d] = %f\n", index, matrix_row_sums[index]);
	index = rand() % SIZE;
	printf("matrix_col_sums[%d] = %f\n", index, matrix_col_sums[index]);
	printf("\n");

	return 0;
}
// #define SIZE 1000
// #define NBR_BENCHMARK_ITERATIONS 5000
// output:
// Taking a row sum of an 1000 by 1000 matrix took an average of 4.578509e-04 s
// Taking a column sum of an 1000 by 1000 matrix took an average of 8.297522e-04 s
// Taking (improved) column sum of an 1000 by 1000 matrix took an average of 5.937005e-04 s
// matrix_row_sums[97] = 12.485320
// matrix_col_sums[911] = -8.742697
// 
// Taking a row sum of an 1000 by 1000 matrix took an average of 1.730820e-04 s
// Taking a column sum of an 1000 by 1000 matrix took an average of 2.219481e-04 s
// Taking (improved) column sum of an 1000 by 1000 matrix took an average of 1.071992e-04 s
// matrix_row_sums[33] = 0.773345
// matrix_col_sums[739] = -37.157414
// 
// Taking a row sum of an 1000 by 1000 matrix took an average of 1.442918e-04 s
// Taking a column sum of an 1000 by 1000 matrix took an average of 2.285175e-04 s
// Taking (improved) column sum of an 1000 by 1000 matrix took an average of 1.416239e-04 s
// matrix_row_sums[179] = -35.065198
// matrix_col_sums[733] = 1.541082

