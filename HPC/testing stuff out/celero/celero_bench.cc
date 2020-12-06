/*
 * Confer the makefile to see how to install Celero.
 */

#include <cstring>
#include "celero/Celero.h"

constexpr size_t szs = 50;
constexpr size_t szb = 1000;

constexpr size_t nmbsmpl = 10;
constexpr size_t nmbiter = 100;

constexpr size_t nbr_rows = 1000;
constexpr size_t nbr_cols = 1000;


CELERO_MAIN

BASELINE(memcpy, baseline, nmbsmpl, nmbiter)
{
	int * a = (int*) malloc(sizeof(int)*2*szs);
	celero::DoNotOptimizeAway(a);
	free(a);
}

BENCHMARK(memcpy, direct_access_small, nmbsmpl, nmbiter)
{
	// Create matrix and fill it with random entries between -1 and 1
	double **matrix = (double**) malloc(nbr_rows * sizeof(double*));
	for (int i = 0; i < nbr_rows; ++i) {
		matrix[i] = (double*) malloc(nbr_cols * sizeof(double));

		for (int j = 0; j < nbr_cols; j++) {
			matrix[i][j] = (double) rand() / RAND_MAX * 2. - 1.;
		}
	}

	// Sum each row of matrix
	double sums[nbr_rows];
	for(int row_index = 0; row_index < nbr_rows; ++row_index) {
		double sum = 0.;
		for(int col_index = 0; col_index < nbr_cols; ++col_index) {
			sum += matrix[row_index][col_index];
		}
		sums[row_index] = sum;
	}
}

BENCHMARK(memcpy, memcpy_small, nmbsmpl, nmbiter)
{
	// Create matrix and fill it with random entries between -1 and 1
	double **matrix = (double**) malloc(nbr_rows * sizeof(double*));
	for (int i = 0; i < nbr_rows; ++i) {
		matrix[i] = (double*) malloc(nbr_cols * sizeof(double));

		for (int j = 0; j < nbr_cols; j++) {
			matrix[i][j] = (double) rand() / RAND_MAX * 2. - 1.;
		}
	}
	// Sum each column of matrix
	double sums[nbr_cols];
	for(int col_index = 0; col_index < nbr_cols; ++col_index) {
		double sum = 0.;
		for(int row_index = 0; row_index < nbr_rows; ++row_index) {
			sum += matrix[row_index][col_index];
		}
		sums[col_index] = sum;
	}
}
