#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define NBR_BENCHMARK_ITERATIONS 10

int main() {
	int *first_1048576_integers = malloc(1048576 * sizeof(int));
	FILE *first_1048576_integers_file;

	struct timespec start_time;
	struct timespec end_time;

	for(size_t i = 0; i < 1048576; i++) {
		first_1048576_integers[i] = i;
	}

	/* Write array to file as entries */
	first_1048576_integers_file = fopen("first_1048576_integers_as_entries.bin", "rw");
	timespec_get(&start_time, TIME_UTC);
	for(size_t i = 0; i < NBR_BENCHMARK_ITERATIONS; i++) {
		fwrite(first_1048576_integers, sizeof(int), 1048576, first_1048576_integers_file);
		fread(first_1048576_integers, sizeof(int), 1048576, first_1048576_integers_file);
		fflush(first_1048576_integers_file);
	}
	timespec_get(&end_time, TIME_UTC);
	fclose(first_1048576_integers_file);
	printf("Writing array to file as entries took on average %e s\n", (end_time.tv_sec + end_time.tv_nsec * 1e-9 - start_time.tv_sec - start_time.tv_nsec * 1e-9) / ((double) NBR_BENCHMARK_ITERATIONS));

	/* Write array to file as array */
	first_1048576_integers_file = fopen("first_1048576_integers_as_array.bin", "rw");
	timespec_get(&start_time, TIME_UTC);
	for(size_t i = 0; i < NBR_BENCHMARK_ITERATIONS; i++) {
		fwrite(first_1048576_integers, 1048576 * sizeof(int), 1, first_1048576_integers_file);
		fread(first_1048576_integers, 1048576 * sizeof(int), 1, first_1048576_integers_file);
		fflush(first_1048576_integers_file);
	}
	timespec_get(&end_time, TIME_UTC);
	fclose(first_1048576_integers_file);
	printf("Writing array to file as array took on average %e s\n", (end_time.tv_sec + end_time.tv_nsec * 1e-9 - start_time.tv_sec - start_time.tv_nsec * 1e-9) / ((double) NBR_BENCHMARK_ITERATIONS));

	printf("\n");
	free(first_1048576_integers);
	return 0;
}
// On the HDD:
// Writing array to file as entries took on average 1.021934e-03 s
// Writing array to file as array took on average 1.047014e-03 s
//
// On the SSD:
// Writing array to file as entries took on average 8.034942e-04 s
// Writing array to file as array took on average 8.080052e-04 s
