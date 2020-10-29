#include "utility_funcs.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define PI (3.1415926535897932)

/* Returns a^n */
double ipow(double a, int n) {
	int i;
	double tmp = 1.0;

	for (i = 0; i < n; i++) {
		tmp *= a;
	}

	return tmp;
}

/* Returns a^2 */
double square(double a) {
	return a * a;
}

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

/* Returns mean of array. */
double mean(double *array, int array_length) {
	return sum(array, array_length) / (double) array_length;
}

/* returns variance of array */
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

double scalar_product(double v1[3], double v2[3]) {
	int i;
	double sum = 0.0;

	for(i = 0; i < 3; i++){
		sum += v1[i] * v2[i];
	}

	return sum;
}

void unit(double v_unit[3], double v[3]) {
	int i;
	double norm;
	norm = sqrt(scalar_product(v,v));

	for(i=0; i<3; i++){
		v_unit[i] = v[i] / norm;
	}
}

double distance(double r1[3], double r2[3]){
	int i;
	double norm;
	double r12[3];

	for(i = 0; i < 3; i++) {
		r12[i] = r1[i] - r2[i];
	}

	norm = sqrt(scalar_product(r12, r12));
	return norm;
}

double norm(double r[3]) {
	return sqrt(scalar_product(r, r));
}

/********************** Model specific functions **********************/

/*
	Two electron wavefunction. 
	Returns probability amplitude of finding electron 1 at r1_vec and electron 2 at r2_vec.
*/
double psi(double r1_vec[3], double r2_vec[3], double alpha) {
	double r1 = norm(r1_vec);
	double r2 = norm(r2_vec);
	double r12 = distance(r1_vec, r2_vec);
	
	return 	exp( -2 * r1 - 2 * r2 + r12 / (2 * (1 + alpha * r12)));
}

/* Local energy */
double E_local(double r1_vec[3], double r2_vec[3], double alpha){
	int i;

	double r1_hat[3];
	double r2_hat[3];

	unit(r1_hat, r1_vec);
	unit(r2_hat, r2_vec);

	double r12_hat[3];
	double r12_vec[3];
	for(i = 0; i < 3; i++){
		r12_hat[i] = r1_hat[i] - r2_hat[i];
		r12_vec[i] = r1_vec[i] - r2_vec[i];
	}
	double r12 = distance(r1_vec, r2_vec);
	
	return
		-4.0
		+ scalar_product(r12_hat, r12_vec) / (r12 * ipow(1.0 + alpha * r12, 2))
		- 1.0 / (r12 * ipow(1.0 + alpha * r12, 3))
		- 1.0 / (4 * ipow(1.0 + alpha * r12, 4))
		+ 1.0 / r12;
}

/* Unnormalized electron probability density from psi^2 */
double rho( double r1[3], double r2[3], double alpha){
	return square(psi(r1, r2, alpha));
}
