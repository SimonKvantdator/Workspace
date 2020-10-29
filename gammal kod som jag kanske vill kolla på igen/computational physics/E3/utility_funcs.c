#include "utility_funcs.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define PI (3.1415926535897932)


/* Returns a^2 */
double square(double a) {
	return a * a;
}

/* Returns mean of array. */
double mean(double *array, int array_length) {
	return sum(array, array_length) / (double) array_length;
}

// double mean(double *array, int array_length) {
// 	int i; // for summing over
// 	double tmp = 0.0;

// 	if (!(array_length > 0)) { // make sure array length is positive
// 		tmp = NAN;
// 		printf("Invalid array_length.\n");
// 	} else if (array_length > 100) {
// 		double *first_half = array;
// 		double *second_half = array + array_length / 2;
// 		double first_half_length = array_length / 2;
// 		double second_half_length = array_length - (array_length / 2);

// 		tmp += mean(first_half, first_half_length) * ((double) first_half_length / array_length);
// 		tmp += mean(second_half, second_half_length) * ((double) second_half_length / array_length);
// 	} else {
// 		for (i = 0; i < array_length; i++) {
// 			tmp += array[i] / array_length;
// 		}
// 	}

// 	return tmp;
// }

/* Returns sum of array using Kahan summation algorithm. */
double sum(double *array, int array_length) {
	int i; // For summing over
	double term;
	double tmp_sum;
	double sum = 0.0; // Prepare the accumulator.
	double compensator = 0.0; // A running compensation for lost low-order bits.

	for (i = 0; i < array_length; i++) {
		term = array[i] - compensator;
		tmp_sum = sum + term;
		compensator = (tmp_sum - sum) - term;
		sum = tmp_sum;
	}

	return sum;
}



/* returns variance of elements in array */
double variance(double *array, int array_length) {
	int i; // for summing over
	double tmp;
	double *array_squared = malloc(array_length * sizeof(double));

	for (i = 0; i < array_length; i++) {
		array_squared[i] = square(array[i]);
	}

	tmp = mean(array_squared, array_length) - square(mean(array, array_length));
	free(array_squared);

	return tmp;
}

double integrand_task1(double x) {
	return x * (1.0 - x);
}

/* probability distribution of x in task 2 */
double f_task2(double x) {
	return sin(PI * x) * PI / 2;
}

double F_inv_task2(double x) {
	return (2 * asin(sqrt(x))) / PI;
}

double integrand_task2(double x) {
	return x * (1.0 - x) / f_task2(x);
}

/* probability distribution of x, y, z in task 3 */
double f_task3(double x, double y, double z) {
	return pow(PI, -3.0 / 2.0) * exp(-(x * x + y * y + z * z));
}

/* (x^2 + x^2 y^2 + x^2 y^2 z^2) */
double integrand_task3(double x, double y, double z) {
	return x * x + x * x * y * y + x * x * y * y * z * z;
}

double autocorrelation(double *f, int f_length, int k) {
	int i; // variable to sum over
	double f_mean = 0.0;
	double numerator = 0.0;
	double denominator = 0.0;

	f_mean = mean(f, f_length);

	for(i = 0; i < f_length - k; i++) {
		numerator += (f[i] - f_mean) * (f[i+k] - f_mean);
	}

	for (i = 0; i < f_length; i++) {
		denominator += (f[i] - f_mean) * (f[i] - f_mean);
	}

	return numerator / denominator;
}

/* j * B ought not to be > length of f */
double Fj(int j, int B, double *f) {
	int i;
	double tmp_sum = 0.0;

	for(i = 1; i < B; i++) {
		tmp_sum = f[i + j * B];
	}
	return tmp_sum / ((double) B);
}

double block_average(int block_size, int block_index, double *array)
{
	int i;
	double tmp_array[block_size]; // to be taken average of, called F_j in pdf

	for (i = 0; i < block_size; i++)
	{
		tmp_array[i] = array[i + block_index * block_size];
	}
	
	return mean(tmp_array, block_size);
}