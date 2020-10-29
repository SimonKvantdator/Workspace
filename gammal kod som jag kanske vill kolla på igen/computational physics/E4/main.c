#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <time.h>

#include "utility_funcs.h"
#include "langevin_verlet.h"

int main(){
	int i,j;
	double one_trajectory_time = 5; /*[ms]*/
	double dt=1e-4;

	int nbr_steps = ceil(one_trajectory_time/dt);
	double x = 0.1; /*[micro m]*/ 
	double v = 2.0; /* [micro m// ms]*/

	double tau = 147.3 * 1e-3; /* [ms] */

	/*G1, G2  Gaussisk probability*/
		/* setup RNG */
		const gsl_rng_type *Type;
		gsl_rng *q;
		gsl_rng_env_setup();
		Type = gsl_rng_default;
		q = gsl_rng_alloc(Type);
		gsl_rng_set(q, time(NULL));

	double sigma =1.0;
	double G1; /* Gaussian distribution mean 0 and deviation sigma */
	double G2;

	double current_time;	


	/************************************************************************/
	int N=1e3 ;/*amount of trajactories to make mean */
	double x_traj[N];
	double v_traj[N];
	double a_traj[N];


	for(i=0; i<N; i++){
		x_traj[i] = 0.1; /*[micro m]*/ 
		v_traj[i] = 2.0; /* [micro m// ms]*/
		a_traj[i] = acc_calc(x,v,tau);
	}

	double *x_array = malloc(nbr_steps * N * sizeof(double));
	double *v_array = malloc(nbr_steps * N * sizeof(double));
	double *time_array = malloc(nbr_steps * N * sizeof(double));

	current_time = 0.0;
	for(i=0 ; i<nbr_steps; i++){

		for(j=0; j<N; j++){
			G1 = gsl_ran_gaussian(q,sigma); /* Gaussian distribution mean 0 and deviation sigma */
			G2 = gsl_ran_gaussian(q,sigma);		
				
			langevin_verlet(&x_traj[j], &v_traj[j], &a_traj[j], dt, tau, G1, G2);

			/* Save x & v for plotting */
			x_array[i + nbr_steps * j] = x_traj[j];
			v_array[i + nbr_steps * j] = v_traj[j];
			time_array[i + nbr_steps * j] = current_time;

		}
		current_time += dt;
	}


	/* Write x_array & v_array to file */
	FILE* x_array_file;
	x_array_file = fopen("histo_x.bin","w");
	FILE* v_array_file;
	v_array_file = fopen("histo_v.bin","w");
	FILE* time_array_file;
	time_array_file = fopen("time.bin","w");

	fwrite(x_array, nbr_steps * N * sizeof(double), 1, x_array_file);
	fwrite(v_array, nbr_steps * N * sizeof(double), 1, v_array_file);
	fwrite(time_array, nbr_steps * N * sizeof(double), 1, time_array_file);

	fclose(x_array_file);
	fclose(v_array_file);
	gsl_rng_free(q);

	return 0;
}


/*****************************************************************************/
/*	FILE* file1;
	file1 = fopen("trajactory1.dat","w");
	current_time=0.0; 
	for(i=0 ; i<nbr_steps; i++){
		G1 = gsl_ran_gaussian(q,sigma); *//* Gaussian distribution mean 0 and deviation sigma */
/*		G2 = gsl_ran_gaussian(q,sigma);		

		langevin_verlet(&x, &v, &a, dt, tau, G1, G2);	
		fprintf(file1 ,"%f \t %f \t %f \n",current_time,x,v);
		current_time += dt;	
	}
	fclose(file1);*/













