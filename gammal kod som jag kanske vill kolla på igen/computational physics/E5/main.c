#include <stdio.h>
//#include <stdlib.h>
#include <math.h>
//#include <gsl/gsl_rng.h>
//#include <gsl/gsl_randist.h>
//#include <time.h>
#define PI (3.1415926535897932)
#include "utility.h"

int main(){
	int i;
	int N = 1000;
	double dr = 1e-2;
	double r[N];
	double n_s[N];
	double V_H[N];

	FILE* file;

	for(i = 0; i < N; i++){
		r[i] = dr + i * dr;
		n_s[i] = exp(-2 * r[i]) / PI; // Hydrogen atom electronic density
	}	
	
	get_Hartee(r, V_H, n_s, N, dr);

	file = fopen("hartee.dat","w");
	for(i = 0; i < N; i++){	
		fprintf(file, "%g \t %g \n", r[i], V_H[i]);
	}
	fclose(file);

	return 0;
}
