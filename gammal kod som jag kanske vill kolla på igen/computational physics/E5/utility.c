#include <gsl/gsl_linalg.h>
#include <stdio.h>
//#include <stdlib.h>
#include <math.h>
//#include <gsl/gsl_rng.h>
//#include <gsl/gsl_randist.h>
//#include <time.h>
#define PI (3.1415926535897932)

#include "utility.h"
void electron_density(double V_H[], double r[], double f[], double n_s[], int N){ /* for hydrogen atom, see eq 15 QS_lecture */
	int i;
	for (i = 0; i < N; i++) {
		n_s[i] = f[i] * f[i] / (4 * PI * r[i] * r[i]);
	}
}

/* Writes Hartree potential to array V_H given electron density n_s, nbr of r-steps N, & r-stepsize dr. */
void get_Hartee(double r[], double V_H[], double n_s[], int N, double dr){
	int i;

	gsl_matrix *D = gsl_matrix_calloc(N, N);
	gsl_vector *b = gsl_vector_calloc(N);	
	gsl_vector *U = gsl_vector_calloc(N);

	/* Producing b = -4 pi r n_s(r), in solving eq */	
	for (i = 0; i < N - 1; i++) { 
		gsl_vector_set(b, i, -4 * PI * r[i] * n_s[i]);
	}
	gsl_vector_set(b, N - 1, 1);	

	/* producing the discritesed (second order) diffrential matrix */
	gsl_matrix_set(D, 0, 0, -2.0 );
	gsl_matrix_set(D, 0, 1, 1.0);
	for(i = 1; i < N - 1; i++) { 
		gsl_matrix_set(D, i, i - 1, 1);
		gsl_matrix_set(D, i, i, -2);
		gsl_matrix_set(D, i, i + 1, 1);
	}
	gsl_matrix_scale(D, 1.0 / (dr * dr));

	gsl_matrix_set(D, N - 1, N - 1, 1);

	int s;
  	gsl_permutation * p = gsl_permutation_alloc(N);
  	gsl_linalg_LU_decomp(D, p, &s);
  	gsl_linalg_LU_solve(D, p, b, U);	

	for (i = 0; i < N; i++) {
		V_H[i] = gsl_vector_get(U, i) / r[i];	
	}

}
