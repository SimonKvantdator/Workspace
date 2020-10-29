#include <gsl/gsl_rng.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <omp.h> // for running loops in parallell
#include "utility_funcs.h"

#define MC_LENGTH ((int) 1e6)

void task1() {
	/* declare some variables */
	int i; // for summing over
	double x;
	double integral = 0.0;
	double error;

	/* initiate some variables */
	int nbr_samples = (int) pow(10, 3); // called N in E3.pdf

	/*allocate some memory */
	double *x_list = malloc(nbr_samples * sizeof(double));
	double *integrand_list = malloc(nbr_samples * sizeof(double));

	/* setup RNG */
	const gsl_rng_type *T;
	gsl_rng *q;
	gsl_rng_env_setup();
	T = gsl_rng_default;
	q = gsl_rng_alloc(T);
	gsl_rng_set(q, time(NULL));

	/* Perform Monte Carlo integration */
	for(i = 0; i < nbr_samples; i++) {
		x = gsl_rng_uniform(q);
		x_list[i] = x;
		integrand_list[i] = integrand_task1(x);
		integral += integrand_task1(x);
	}
	integral /= nbr_samples;
	error = sqrt(variance(integrand_list, nbr_samples) / nbr_samples);

	/* Print some info */
	printf("\n");
	printf("################ TASK 1 ################\n");
	printf("N 	 = %d\n", nbr_samples);
	printf("integral = %f (true value %f)\n", integral, 1.0 / 6.0);
	printf("error 	 = %f (1 sigma)\n", error);

	/* free memory */
	gsl_rng_free(q);
}

void task2() {
	/* Declare some variables */
	int i; // for summing over
	double integral;
	double x;
	double error;

	/* Initiate some variables */
	int nbr_samples = (int) pow(10, 3); // called N in E3.pdf
	int nbr_parameters = 1;
	double parameters[] = {(double) nbr_samples};

	/* Allocate some memory */
	double *x_list = malloc(nbr_samples * sizeof(double));
	double *integrand_list = malloc(nbr_samples * sizeof(double));

	/* Declare some files */
	FILE *x_list_file;
	FILE *integrand_list_file;
	FILE *parameters_file;

	/* setup RNG */
	const gsl_rng_type *T;
	gsl_rng *q;
	gsl_rng_env_setup();
	T = gsl_rng_default;
	q = gsl_rng_alloc(T);
	gsl_rng_set(q, time(NULL));


	/* Perform Monte Carlo integration */
	integral = 0.0;
	for(i = 0; i < nbr_samples; i++) {
		x = F_inv_task2(gsl_rng_uniform(q));
		x_list[i] = x;
		integrand_list[i] = integrand_task2(x);
		integral += integrand_task2(x);
	}
	integral /= nbr_samples;
	error = sqrt(variance(integrand_list, nbr_samples) / nbr_samples);


	/* Print some info */
	printf("\n");
	printf("################ TASK 2 ################\n");
	printf("N 	 = %d\n", nbr_samples);
	printf("integral = %f (true value %f)\n", integral, 1.0 / 6.0);
	printf("error 	 = %f (1 sigma)\n", error);

	/* Write x_list & integrands_list to file */
	x_list_file = fopen("x_list_task2.bin", "w");
	integrand_list_file = fopen("integrand_list_task2.bin", "w");
	parameters_file = fopen("parameters_task2.bin", "w");

	fwrite(x_list, nbr_samples * sizeof(double), 1, x_list_file);
	fwrite(integrand_list, nbr_samples * sizeof(double), 1, integrand_list_file);
	fwrite(parameters, nbr_parameters * sizeof(double), 1, parameters_file);
	
	fclose(x_list_file);
	fclose(integrand_list_file);
	fclose(parameters_file);

	/* free memory */
	free(x_list);
	free(integrand_list);
	gsl_rng_free(q);
}

void task3() {
	/* Declare some variables */
	int nbr_accepted; // # of proposed steps that are accepted 
	float integral;

	/* Initiate some variables */
	int nbr_samples = (int) pow(10, 5); // called N in E3.pdf
	int nbr_warmup_samples = (int) pow(10, 4);
	int nbr_walkers = (int) pow(2, 8);
	double Delta = 2.2;

	/* setup RNG */
	const gsl_rng_type *T;
	gsl_rng *q;
	gsl_rng_env_setup();
	T = gsl_rng_default;
	q = gsl_rng_alloc(T);
	gsl_rng_set(q, time(NULL));


	/* Perform Monte Carlo integration w/ Metropolis algorithm */
	integral = 0.0;
	nbr_accepted = 0;
	
	omp_set_num_threads(4);
    #pragma omp parallel for // we can run the walkers parallell
	for(int i = 0; i < nbr_walkers; i++) {
		/* Declare variables which are "local to this core" */
		double tmp_integral = 0.0;
		int tmp_nbr_accepted = 0;
		double x_current = 0.0;
		double y_current = 0.0;
		double z_current = 0.0;
		double x_new;
		double y_new;
		double z_new;

		/* Warmup Metropolis algorithm */
		for(int j = 0; j < nbr_warmup_samples; j++) {
			/* Propose step */
			x_new = x_current + Delta * (gsl_rng_uniform(q) - 0.5);
			y_new = y_current + Delta * (gsl_rng_uniform(q) - 0.5);
			z_new = z_current + Delta * (gsl_rng_uniform(q) - 0.5);

			/* accept proposal? */
			if (f_task3(x_new, y_new, z_new) / f_task3(x_current, y_current, z_current) > gsl_rng_uniform(q)) {

				x_current = x_new;
				y_current = y_new;
				z_current = z_new;
			}
		}

		/* Run Metropolis algorithm */
		for(int j = 0; j < nbr_samples; j++) {
			/* Propose step */
			x_new = x_current + Delta * (gsl_rng_uniform(q) - 0.5);
			y_new = y_current + Delta * (gsl_rng_uniform(q) - 0.5);
			z_new = z_current + Delta * (gsl_rng_uniform(q) - 0.5);

			/* accept proposal? */
			if (f_task3(x_new, y_new, z_new) / f_task3(x_current, y_current, z_current) > gsl_rng_uniform(q)) {
				x_current = x_new;
				y_current = y_new;
				z_current = z_new;

				tmp_nbr_accepted++;
			}
			tmp_integral += integrand_task3(x_current, y_current, z_current);
		}
		integral += tmp_integral / nbr_samples;
		nbr_accepted += tmp_nbr_accepted;
	}
	integral /= nbr_walkers;

	/* Print some info */
	printf("\n");
	printf("################ TASK 3 ################\n");
	printf("# of walkers            = %d\n", nbr_walkers);
	printf("# of samples per walker = %d\n", nbr_samples);
	printf("accepted ratio          = %.2f\n", (double) nbr_accepted / (nbr_walkers * nbr_samples));
	printf("integral                = %f (true value %f)\n", integral, 7.0 / 8.0);

	/* free memory */
	gsl_rng_free(q);
}

void task4() {
	/*Declare variables */
	double autocorr;
	double MC_mean;
	double var_f;
	int nbr_block_sizes = 10000;
    int count = 0; // BODGE
	int tmp;
    int i, k; // for summing over
	int s;

	/* Allocate memory for large arrays */
	double *MC_list = malloc(MC_LENGTH * sizeof(double));
	double *s_from_block_average = malloc(nbr_block_sizes * sizeof(double));
	s_from_block_average[0] = NAN; // no block of size 0

    FILE *MC_file;
	FILE *s_from_block_average_file;

	/* Read MC.txt */
    MC_file = fopen("MC.txt","r");
	for(i = 0; i < MC_LENGTH; i++) {
		tmp = fscanf(MC_file, "%lf", &(MC_list[count++]));
		tmp++; // this is a bodgy way of supressing warnings about fscanf return value unused
    }
    fclose(MC_file);


	/* Determine s from autocorrelation */
	k = 0;
	autocorr = 1.0;
	while (autocorr > exp(-2)) {
		k++;
		autocorr = autocorrelation(MC_list, MC_LENGTH, k);
	}
	s = k; // save for printing


	/* Shift data in MC_list to have mean 0.0 */
	MC_mean = mean(MC_list, MC_LENGTH);
	for (i = 0; i < MC_LENGTH; i++) {
		MC_list[i] -= MC_mean;
	}

	/* Determine s from block averaging */
	var_f = variance(MC_list, MC_LENGTH);
	omp_set_num_threads(4);
    #pragma omp parallel for // we can run the blocks parallell
	for(int block_size = 1; block_size < nbr_block_sizes; block_size++) { // block_size called B in pdf
		int j; // for summing over
		int nbr_blocks = MC_LENGTH / block_size;
		double F[nbr_blocks];

		for(j = 0; j < nbr_blocks; j++) { // j is block index
			F[j] = block_average(block_size, j, MC_list);
		}

		s_from_block_average[block_size] = block_size * variance(F, nbr_blocks) / var_f;
	}


	/* Print some info */
	printf("\n");
	printf("################ TASK 4 ################\n");
	printf("from autocorrelation, s = %d\n", s);
	double S = mean(s_from_block_average + (9 * nbr_block_sizes) / 10, (1 * nbr_block_sizes) / 10); // bodgy way of taking average of last 1/10 of array
	printf("from block averaging, s = %f\n", S);

	/* Write B * Var[F] / Var[f] to file */
	s_from_block_average_file = fopen("s_from_block_average_task4.bin", "w");
	fwrite(s_from_block_average, nbr_block_sizes * sizeof(double), 1, s_from_block_average_file);
	fclose(s_from_block_average_file);

	free(MC_list);
}

int main() {

	task1();

	task2();

	task3();

	task4();

	return 0;
}



