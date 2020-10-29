#include <gsl/gsl_rng.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <omp.h> // for running loops in parallell
#include "utility_funcs.h"

/* 
	Given an ansatz parameter alpha, forms the wavefunction 
	psi(r1, r2) = exp(-2 r1 -2 r2 + r12 / (2 (1 + alpha r12)))
	and calculates its energy in the He-potential using MCMC.
	talk is a boolean int, if != 0, the method prints the results.
*/
void task1(double alpha, int nbr_samples, int talk) {
	/* Declare some variables */
	int nbr_accepted; // # of proposed steps that are accepted 
	double energy_integral;

	/* Initiate some variables */
	int nbr_warmup_samples = 400; // 200 looked decent from plot
	int nbr_walkers = (int) pow(2, 6);
	double Delta = 1.0; // tuned s.th accepted ratio = 0.5
    double zero_vector[] = {0.0, 0.0, 0.0};
	double parameters[] = {alpha, Delta, nbr_samples, nbr_walkers};
	int nbr_parameters = 4;

    /* Allocate memory for samples */
    double *E_local_list = malloc(nbr_samples * nbr_walkers * sizeof(double)); // E_local_list[sample_index * nbr_walkers + walker_index]
    double *r1_list = malloc(nbr_samples * nbr_walkers * sizeof(double));
    double *r2_list = malloc(nbr_samples * nbr_walkers * sizeof(double));
	double *theta_list = malloc(nbr_samples * nbr_walkers * sizeof(double));

	/* Declare files for writing lists to */
	FILE *E_local_list_file;
	FILE *r1_list_file;
	FILE *r2_list_file;
	FILE *theta_list_file;
    FILE *parameters_file;

	/* setup RNG */
	const gsl_rng_type *T;
	gsl_rng *q;
	gsl_rng_env_setup();
	T = gsl_rng_default;
	q = gsl_rng_alloc(T);
	gsl_rng_set(q, time(NULL));


	/* Perform Monte Carlo integration w/ Metropolis algorithm */
	nbr_accepted = 0;
	
	omp_set_num_threads(4);
    #pragma omp parallel for // we can run the walkers parallell
	for(int i = 0; i < nbr_walkers; i++) {
		/* Declare variables which are "local to this core" */
        int j, k;
		int tmp_nbr_accepted = 0;
		double r1_current[3];
		double r2_current[3];
		for (k = 0; k < 3; k++) {
			r1_current[k] = 10.0 * (gsl_rng_uniform(q) - 0.5); // randomly initiate the electrons far from the nucleus
			r2_current[k] = 10.0 * (gsl_rng_uniform(q) - 0.5);
		}
		double r1_new[3];
		double r2_new[3];

		/* Warmup Metropolis algorithm */
		for (j = 0; j < nbr_warmup_samples; j++) {
			/* Propose step */
            for(k = 0; k < 3; k++) {
                r1_new[k] = r1_current[k] + Delta * (gsl_rng_uniform(q) - 0.5);
                r2_new[k] = r2_current[k] + Delta * (gsl_rng_uniform(q) - 0.5);
            }

			/* accept proposal? */
			if (rho(r1_new, r2_new, alpha) / rho(r1_current, r2_current, alpha) > gsl_rng_uniform(q)) {
                for (k = 0; k < 3; k++) {
                    r1_current[k] = r1_new[k];
                    r2_current[k] = r2_new[k];
                }
			}
		}

		/* Run Metropolis algorithm */
		for (j = 0; j < nbr_samples; j++) {
			/* Propose step */
			for (k = 0; k < 3; k++) {
                r1_new[k] = r1_current[k] + Delta * (gsl_rng_uniform(q) - 0.5);
                r2_new[k] = r2_current[k] + Delta * (gsl_rng_uniform(q) - 0.5);
            }

			/* accept proposal? */
			if (rho(r1_new, r2_new, alpha) / rho(r1_current, r2_current, alpha) > gsl_rng_uniform(q)) {
				for (k = 0; k < 3; k++) {
                    r1_current[k] = r1_new[k];
                    r2_current[k] = r2_new[k];
                }

				tmp_nbr_accepted++;
			}

			/* Save relevant quantities */
			E_local_list[j * nbr_walkers + i] = E_local(r1_current, r2_current, alpha);
            r1_list[j * nbr_walkers + i] = distance(zero_vector, r1_current);
            r2_list[j * nbr_walkers + i] = distance(zero_vector, r2_current);
			theta_list[j * nbr_walkers + i] = acos(scalar_product(r1_current, r2_current) / (norm(r1_current) * norm(r2_current)));
		}
		nbr_accepted += tmp_nbr_accepted;
	}
    energy_integral = mean(E_local_list, nbr_samples * nbr_walkers); // expected value of energy


	/* Print some info */
	if (talk != 0) {
		printf("\n");
		printf("################ TASK 1 ################\n");
		printf("alpha                   = %f\n", alpha);
		printf("# of walkers            = %d\n", nbr_walkers);
		printf("# of samples per walker = %d\n", nbr_samples);
		printf("accepted ratio          = %.2f\n", (double) nbr_accepted / (nbr_walkers * nbr_samples));
		printf("energy integral         = %f\n", energy_integral);
	}

    /* Write lists to file */
	E_local_list_file = fopen("E_local_list_task1.bin", "w");
	r1_list_file = fopen("r1_list_task1.bin", "w");
	r2_list_file = fopen("r2_list_task1.bin", "w");
	theta_list_file = fopen("theta_list_task1.bin", "w");
    parameters_file = fopen("parameters_task1.bin", "w");

	fwrite(E_local_list, nbr_samples * nbr_walkers * sizeof(double), 1, E_local_list_file);
	fwrite(r1_list, nbr_samples * nbr_walkers * sizeof(double), 1, r1_list_file);
	fwrite(r2_list, nbr_samples * nbr_walkers * sizeof(double), 1, r2_list_file);
	fwrite(theta_list, nbr_samples * nbr_walkers * sizeof(double), 1, theta_list_file);
	fwrite(parameters, nbr_parameters * sizeof(double), 1, parameters_file);
	
	fclose(E_local_list_file);
	fclose(r1_list_file);
	fclose(r2_list_file);
	fclose(theta_list_file);
    fclose(parameters_file);

	/* free memory */
	gsl_rng_free(q);
    free(E_local_list);
	free(r1_list);
	free(r2_list);
	free(theta_list);
}


/*
	Given an ansatz parameter alpha, forms the wavefunction 
	psi(r1, r2) = exp(-2 r1 -2 r2 + r12 / (2 (1 + alpha r12)))
	and calculates its energy in the He-potential using MCMC.
	talk is a boolean int, if != 0, the method prints the results.
*/
void task2a(double alpha, int talk) {
	/* Initiate some variables */
	int nbr_samples = 1e6;
	int nbr_accepted = 0; // # of proposed steps that are accepted
	double Delta = 1.0; // tuned s.th accepted ratio = 0.5
	double integral = 0.0; // for integrating up E_local
	double r1_current[3] = {0.0, 10.0, 0.0}; // for storing the current location in the Metropolis algorithm
	double r2_current[3] = {10.0, 0.0, 0.0}; // for storing the current location in the Metropolis algorithm
	double parameters[] = {alpha, Delta, nbr_samples};
	int nbr_parameters = 3;

	/* Declare some variables */
	int i, j; // for summing over
	double r1_new[3]; // for proposing steps in Metropolis algorithm
	double r2_new[3]; // for proposing steps in Metropolis algorithm

    /* Allocate memory for samples */
    double *E_local_list = malloc(nbr_samples * sizeof(double)); // E_local_list[sample_index * nbr_walkers + walker_index]
 
	/* Declare files for writing lists to */
	FILE *E_local_list_file;
    FILE *parameters_file;

	/* setup RNG */
	const gsl_rng_type *T;
	gsl_rng *q;
	gsl_rng_env_setup();
	T = gsl_rng_default;
	q = gsl_rng_alloc(T);
	gsl_rng_set(q, time(NULL));


	/* Perform Monte Carlo integration w/ Metropolis algorithm */
	for (i = 0; i < nbr_samples; i++) {
		/* Propose step */
		for (j = 0; j < 3; j++) {
			r1_new[j] = r1_current[j] + Delta * (gsl_rng_uniform(q) - 0.5);
			r2_new[j] = r2_current[j] + Delta * (gsl_rng_uniform(q) - 0.5);
		}

		/* accept proposal? */
		if (rho(r1_new, r2_new, alpha) / rho(r1_current, r2_current, alpha) > gsl_rng_uniform(q)) {
			for (j = 0; j < 3; j++) {
				r1_current[j] = r1_new[j];
				r2_current[j] = r2_new[j];
			}

			nbr_accepted++;
		}

		/* Save relevant quantities */
		E_local_list[i] = E_local(r1_new, r2_new, alpha);
	}
    integral = sum(E_local_list, nbr_samples) / nbr_samples;


	/* Print some info */
	if (talk != 0) {
		printf("\n");
		printf("################ TASK 2 ################\n");
		printf("alpha                   = %f\n", alpha);
		printf("# of walkers            = %d\n", 1);
		printf("# of samples per walker = %d\n", nbr_samples);
		printf("accepted ratio          = %.2f\n", (double) nbr_accepted / nbr_samples);
		printf("energy integral         = %f\n", integral);
	}

    /* Write lists to file */
	E_local_list_file = fopen("E_local_list_task2.bin", "w");
    parameters_file = fopen("parameters_task2.bin", "w");

	fwrite(E_local_list, nbr_samples * sizeof(double), 1, E_local_list_file);
	fwrite(parameters, nbr_parameters * sizeof(double), 1, parameters_file);
	
	fclose(E_local_list_file);
    fclose(parameters_file);

	/* free memory */
	gsl_rng_free(q);
    free(E_local_list);
}


/*
	Calculates statistical inefficiency given the data from task2a.
*/
void task2b() {
	/* Initiate some variables */
	int nbr_parameters = 3; // a bit bodge but OK
	double parameters[3];

	/* Declare some files to read from */
	FILE *parameters_file;
	FILE *E_local_list_file;

	/* Read parameters from file */
	parameters_file = fopen("parameters_task2.bin", "r");
	int tmp; // this is a bodgy way of suppressing warning about fread:s return value unused
	tmp = fread(parameters, nbr_parameters * sizeof(double), 1, parameters_file);
	int nbr_samples = parameters[2];

	/* Read local energies from file */
	double *E_local_list = malloc(nbr_samples * sizeof(double));
	E_local_list_file = fopen("E_local_list_task2.bin", "r");
	tmp = fread(E_local_list, nbr_samples * sizeof(double), 1, E_local_list_file);
	tmp++;

	fclose(E_local_list_file);
    fclose(parameters_file);

	/* Declare some variables now that we know nbr_samples */
	int nbr_block_sizes = 10000;
	double var_E_local;
	int autocorr_length = 600;
	double denominator; // used in calculating autocorrelation
	double E_local_mean;
	double *autocorr = malloc(autocorr_length * sizeof(double));
	double *s_from_block_average = malloc(nbr_block_sizes * sizeof(double));

	/* Declare some files to write to */
	FILE *autocorr_file;
	FILE *s_from_block_average_file;


	/* Calculate autocorrelation */
	E_local_mean = mean(E_local_list, nbr_samples);
	var_E_local = variance(E_local_list, nbr_samples);
	denominator = var_E_local;
	omp_set_num_threads(4);
    #pragma omp parallel for
	for (int i = 0; i < autocorr_length; i++) {
		int j; // for summing over
		double numerator = 0.0;

		for(j = 0; j < nbr_samples - i; j++) {
			numerator += E_local_list[j] * E_local_list[j+i] - ipow(E_local_mean, 2);
		}
		numerator /= (double) (nbr_samples - i);

		autocorr[i] = numerator / denominator;
	}

	/* Calculate s(block_size) = block_size * var_F / var_f */
	omp_set_num_threads(4);
    #pragma omp parallel for
	for(int block_size = 1; block_size < nbr_block_sizes; block_size++) { // block_size called B in pdf
		int j; // for summing over
		int nbr_blocks = nbr_samples / block_size;
		double E_local_block_average[nbr_blocks];

		for(j = 0; j < nbr_blocks; j++) { // j is block index
			E_local_block_average[j] = block_average(block_size, j, E_local_list);
		}

		s_from_block_average[block_size] = block_size * variance(E_local_block_average, nbr_blocks) / var_E_local;
	}


	/* Save autocorrelationn & s(block_size) to file */
	autocorr_file = fopen("autocorrelations_task2.bin", "w");
	s_from_block_average_file = fopen("s_task2.bin", "w");

	fwrite(autocorr, autocorr_length * sizeof(double), 1, autocorr_file);
	fwrite(s_from_block_average, nbr_block_sizes * sizeof(double), 1, s_from_block_average_file);

	fclose(autocorr_file);
	fclose(s_from_block_average_file);

	/* free memory */
	free(E_local_list);
	free(autocorr);
	free(s_from_block_average);
}


/* 
	Calculates the energy using MCMC for a list of different alpha:s.
	talk is a boolean int, if != 0, the method prints the results.
*/
void task3(int talk) {
	/* Declare some variables */
	int i, j, k; // for summing over

	/* Initiate some variables */
	int nbr_samples_task1 = 1e6;
	int nbr_parameters = 4; // # of parameters in task 1
	int statistical_inefficiency = 8; // we assume this is alpha-independent
	int alphas_length = 100;
	double max_alpha = 0.25;
	double min_alpha = 0.05;
	double alphas[alphas_length]; // wavefunction ansatz parameter list

	/* alpha_list is equidistant points from 0.05 to 0.25 */
	for (i = 0; i < alphas_length ; i++) {
		alphas[i] = ((double) i / (alphas_length - 1)) * (max_alpha - min_alpha) + min_alpha;
	}

	/* Declare some more variables */
	double parameters[nbr_parameters]; // parameters from task 1

	FILE *parameters_file;
	FILE *E_local_list_file;
	FILE *alphas_file;
	FILE *energies_file;
    FILE *variances_file;

	/* Read parameters from task 1 */
	task1(alphas[0], nbr_samples_task1, 0); // run task1 once to update parameters_task1.bin
	parameters_file = fopen("parameters_task1.bin", "r"); // {alpha, Delta, nbr_samples, nbr_walkers}
	int tmp = fread(parameters, nbr_parameters * sizeof(double), 1, parameters_file);
	tmp++; // this is a bodgy way of suppressing warning about fread:s return value being unused
	fclose(parameters_file);

	/* Initiate some more variables from parameters from task1 */
	int nbr_samples = parameters[2];
	int nbr_walkers = parameters[3];
	double *E_local_list = malloc(nbr_samples * nbr_walkers * sizeof(double)); // E_local_list[sample_index * nbr_walkers + walker_index]
	double *single_walker_reduced_list = malloc((nbr_samples / statistical_inefficiency) * sizeof(double)); // this is the array we take the variance of to estimate the error
	double *energies = malloc(alphas_length * nbr_walkers * sizeof(double)); // estimated energy for each walker. energies[walker_index + alpha_index * nbr_walkers
	double *variances = malloc(alphas_length * nbr_walkers * sizeof(double)); // estimated variance for each walker. variances[walker_index + alpha_index * nbr_walkers

	/* Print header for info */
	if (talk != 0) {
		printf("\n################ TASK 3 ################\n");
		printf("# of alphas  = %d\n", alphas_length);
		printf("# of samples = %d\n", nbr_samples);
		printf("# of walkers = %d\n", nbr_walkers);
		printf("---------------------------------\n");
	}


	/* Loop over the different alpha:s */
	for (i = 0; i < alphas_length; i++) {

		/* Run metropolis algorithm */
		task1(alphas[i], nbr_samples_task1, 0);

		/* Get the local energies we just calculated from file */
		E_local_list_file = fopen("E_local_list_task1.bin", "r");
		tmp = fread(E_local_list, nbr_samples * nbr_walkers * sizeof(double), 1, E_local_list_file);
		tmp++; // this is a bodgy way of suppressing warning about fread:s return value being unused
		fclose(E_local_list_file);

		
		/* Loop over walkers */
		for (j = 0; j < nbr_walkers; j++) {

			/* Define an array as every s:th element of E_local_list for a specific walker */
			for (k = 0; k < nbr_samples / statistical_inefficiency; k++) {
				single_walker_reduced_list[k] = E_local_list[statistical_inefficiency * k * nbr_walkers + j]; // sample_index = statistical_inefficiency * k
			}

			/* Calculate energy & variance for each walker */
			energies[j + i * nbr_walkers] = mean(single_walker_reduced_list, nbr_samples / statistical_inefficiency);
			variances[j + i * nbr_walkers] = variance(single_walker_reduced_list, nbr_samples / statistical_inefficiency);
		}
		
		/* Print some info */
		if (talk != 0) {
			printf("alpha                  = %f\n", alphas[i]);
			printf("energy from 1st walker = %f\n", energies[0 + i * nbr_walkers]);
			printf("error  from 1st walker = %f\n", sqrt(variances[0 + i * nbr_walkers] / (double) (nbr_samples / statistical_inefficiency)));
			printf("---------------------------------\n");
		}


		/* Write lists to file */
		alphas_file = fopen("alphas_task3.bin", "w");
		energies_file = fopen("energies_task3.bin", "w");
		variances_file = fopen("variances_task3.bin", "w");

		fwrite(alphas, alphas_length * sizeof(double), 1, alphas_file);
		fwrite(energies, alphas_length * nbr_walkers * sizeof(double), 1, energies_file);
		fwrite(variances, alphas_length * nbr_walkers * sizeof(double), 1, variances_file);
		
		fclose(alphas_file);
		fclose(energies_file);
		fclose(variances_file);
	}

	/* Free some memory */
	free(E_local_list);
	free(single_walker_reduced_list);
	free(energies);
	free(variances);
}


/* Returns alpha-derivative of log(psi(r1, r2, alpha)) using symmetric difference quotient with stepsize step. */
double D_alpha_log_psi(double r1[3], double r2[3], double alpha, double step) {
	return (log(psi(r1, r2, alpha + step)) - log(psi(r1, r2, alpha - step))) / (2 * step);
}


/*
	Runs Monte Carlo Markov chain Metropolis algorithm to find
	D_alpha E = <E_local * D_alpha log(psi)> - <E_local> * <D_alpha log(psi)>.
	step is alpha-stepsize used in calculating D_alpha log(psi).
	talk is a boolean int, if != 0, the method prints the result.
*/
double D_alpha_E(double alpha, double step, int nbr_samples, int nbr_walkers, int talk) {
	/* Declare some variables */
	int nbr_accepted; // # of proposed steps that are accepted
	double energy_gradient;

	/* Initiate some variables */
	int nbr_warmup_samples = 200;
	double Delta = 1.0; // tuned s.th acceptance ratio is around 0.5

    /* Allocate memory for samples */
    double *E_local_list = malloc(nbr_samples * nbr_walkers * sizeof(double)); // E_local_list[sample_index * nbr_walkers + walker_index]
	double *D_alpha_log_psi_list = malloc(nbr_samples * nbr_walkers * sizeof(double)); // D_alpha_log_psi_list[sample_index * nbr_walkers + walker_index]
	double *E_local_D_alpha_log_psi_list = malloc(nbr_samples * nbr_walkers * sizeof(double)); // E_local_D_alpha_log_psi_list[sample_index * nbr_walkers + walker_index]

	/* setup RNG */
	const gsl_rng_type *T;
	gsl_rng *q;
	gsl_rng_env_setup();
	T = gsl_rng_default;
	q = gsl_rng_alloc(T);
	gsl_rng_set(q, time(NULL));


	/* Perform Monte Carlo integration w/ Metropolis algorithm */
	nbr_accepted = 0;
	
	omp_set_num_threads(4);
    #pragma omp parallel for // we can run the walkers parallell
	for(int i = 0; i < nbr_walkers; i++) {
		/* Declare variables which are "local to this core" */
        int j, k;
		int tmp_nbr_accepted = 0;
		double r1_current[3];
		double r2_current[3];
		for (k = 0; k < 3; k++) {
			r1_current[k] = 10.0 * (gsl_rng_uniform(q) - 0.5); // randomly initiate the electrons far from the nucleus
			r2_current[k] = 10.0 * (gsl_rng_uniform(q) - 0.5);
		}
		double r1_new[3];
		double r2_new[3];

		/* Warmup Metropolis algorithm */
		for (j = 0; j < nbr_warmup_samples; j++) {
			/* Propose step */
            for(k = 0; k < 3; k++) {
                r1_new[k] = r1_current[k] + Delta * (gsl_rng_uniform(q) - 0.5);
                r2_new[k] = r2_current[k] + Delta * (gsl_rng_uniform(q) - 0.5);
            }

			/* accept proposal? */
			if (rho(r1_new, r2_new, alpha) / rho(r1_current, r2_current, alpha) > gsl_rng_uniform(q)) {
                for (k = 0; k < 3; k++) {
                    r1_current[k] = r1_new[k];
                    r2_current[k] = r2_new[k];
                }
			}
		}

		/* Run Metropolis algorithm */
		for (j = 0; j < nbr_samples; j++) {
			/* Propose step */
			for (k = 0; k < 3; k++) {
                r1_new[k] = r1_current[k] + Delta * (gsl_rng_uniform(q) - 0.5);
                r2_new[k] = r2_current[k] + Delta * (gsl_rng_uniform(q) - 0.5);
            }

			/* accept proposal? */
			if (rho(r1_new, r2_new, alpha) / rho(r1_current, r2_current, alpha) > gsl_rng_uniform(q)) {
				for (k = 0; k < 3; k++) {
                    r1_current[k] = r1_new[k];
                    r2_current[k] = r2_new[k];
                }

				tmp_nbr_accepted++;
			}

			/* Save relevant quantities */
			E_local_list[j * nbr_walkers + i] = E_local(r1_current, r2_current, alpha);
			D_alpha_log_psi_list[j * nbr_walkers + i] = D_alpha_log_psi(r1_current, r2_current, alpha, step);
			E_local_D_alpha_log_psi_list[j * nbr_walkers + i] = E_local_list[j * nbr_walkers + i] * D_alpha_log_psi_list[j * nbr_walkers + i];
		}
		nbr_accepted += tmp_nbr_accepted;
	}


	energy_gradient = 2 * (mean(E_local_D_alpha_log_psi_list, nbr_samples * nbr_walkers)
							- mean(E_local_list, nbr_samples * nbr_walkers)
							* mean(D_alpha_log_psi_list, nbr_samples * nbr_walkers)); // this is a bodgy way of allowing us to free variables before returning.

	if (talk != 0) {
		printf("energy          = %f [a.u.]\n", mean(E_local_list, nbr_samples * nbr_walkers));
		printf("energy gradient = %f [a.u.]\n", energy_gradient);
	}


	/* free memory */
	gsl_rng_free(q);
    free(E_local_list);
	free(D_alpha_log_psi_list);
	free(E_local_D_alpha_log_psi_list);

	/* Return RHS of (36) in QS_lecturenotes.pdf */
	return energy_gradient;
}


/*
	Perform stochastich gradient descent to find optimal alpha and lowest energy.
	talk is a boolean int, if != 0, the method prints the results.
*/
void task4(double alpha_max, double alpha_min, int talk) {
	int i, j; // for summing over

	/* Initiate MCMC parameters */
	double step = 0.01; // alpha-stepsize
	int nbr_samples = 1e3; // # of samples per walker to be used when evaluating D_alpha_E with MCMC
	int nbr_walkers = (int) ipow(2, 3); // # of walkers to be used when evaluating D_alpha_E with MCMC

	/* Initiate & declare stochastic gradient descent parameters */
	int nbr_iterations = 1e4;
	double alpha;
	double gradient; // alpha-gradient of E
	double A = 1.0; // as defined in (8) in H2b.pdf
	double gamma; // as defined in (8) in H2b.pdf
	double beta; // as defined in (8) in H2b.pdf, determines how quickly we converge to a stationary alpha
	double *alphas = malloc(nbr_iterations * nbr_walkers * sizeof(double)); // alphas[iteration_index + walker_index * nbr_iterations]
	

	/* Parameters for saving to file */
	int nbr_parameters = 3;
	double parameters[] = {nbr_iterations, nbr_walkers, nbr_samples};

	/* Declare files */
	FILE *alphas_file;
	FILE *parameters_file;


	/* Perform stochastic gradient descent */
	for (i = 0; i < nbr_walkers; i++) {
	alpha = alpha_min + (alpha_max - alpha_min) * (double) i / nbr_walkers; // Initial guess
		for (j = 0; j < nbr_iterations; j++) {
			gradient = D_alpha_E(alpha, step, nbr_samples, (int) ipow(2, 6), 0); // 2^6 walkers

			/* Update alpha */
			beta = 1.0;
			gamma = A * pow((double) j + 1, -beta); // +1 to avoid zero division

			alphas[j + i * nbr_iterations] = alpha;
			alpha = alpha - gamma * gradient;
		}
	}


	/* Print info */
	if (talk != 0) {
		printf("\n################ TASK 4 ################\n");
		printf("beta = %f\n", beta);
		D_alpha_E(alpha, step, nbr_samples * 100, nbr_walkers, 1);
		printf("alpha           = %f [a.u.]\n", alpha);
	}

	/* Save relevant arrays to file */
	alphas_file = fopen("alphas_task4.bin", "w");
	parameters_file = fopen("parameters_task4.bin", "w");

	fwrite(alphas, nbr_iterations * nbr_walkers * sizeof(double), 1, alphas_file);
	fwrite(parameters, nbr_parameters * sizeof(double), 1, parameters_file);

	fclose(alphas_file);
	fclose(parameters_file);

	/* Free memory */
	free(alphas);
}


void task5(double alpha, int nbr_samples, int talk) {
	/* Declare some variables */
	int nbr_accepted; // # of proposed steps that are accepted 
	double energy_integral;

	/* Initiate some variables */
	int nbr_warmup_samples = 400; // 200 looked decent from plot
	int nbr_walkers = (int) pow(2, 6);
	double Delta = 1.0; // tuned s.th accepted ratio = 0.5

    /* Allocate memory for samples */
    double *E_local_list = malloc(nbr_samples * nbr_walkers * sizeof(double)); // E_local_list[sample_index * nbr_walkers + walker_index]

	/* Declare files for writing lists to */
	FILE *E_local_list_file;

	/* setup RNG */
	const gsl_rng_type *T;
	gsl_rng *q;
	gsl_rng_env_setup();
	T = gsl_rng_default;
	q = gsl_rng_alloc(T);
	gsl_rng_set(q, time(NULL));


	/* Perform Monte Carlo integration w/ Metropolis algorithm */
	nbr_accepted = 0;
	
	omp_set_num_threads(4);
    #pragma omp parallel for // we can run the walkers parallell
	for(int i = 0; i < nbr_walkers; i++) {
		/* Declare variables which are "local to this core" */
        int j, k;
		int tmp_nbr_accepted = 0;
		double r1_current[3];
		double r2_current[3];
		for (k = 0; k < 3; k++) {
			r1_current[k] = 10.0 * (gsl_rng_uniform(q) - 0.5); // randomly initiate the electrons far from the nucleus
			r2_current[k] = 10.0 * (gsl_rng_uniform(q) - 0.5);
		}
		double r1_new[3];
		double r2_new[3];

		/* Warmup Metropolis algorithm */
		for (j = 0; j < nbr_warmup_samples; j++) {
			/* Propose step */
            for(k = 0; k < 3; k++) {
                r1_new[k] = r1_current[k] + Delta * (gsl_rng_uniform(q) - 0.5);
                r2_new[k] = r2_current[k] + Delta * (gsl_rng_uniform(q) - 0.5);
            }

			/* accept proposal? */
			if (rho(r1_new, r2_new, alpha) / rho(r1_current, r2_current, alpha) > gsl_rng_uniform(q)) {
                for (k = 0; k < 3; k++) {
                    r1_current[k] = r1_new[k];
                    r2_current[k] = r2_new[k];
                }
			}
		}

		/* Run Metropolis algorithm */
		for (j = 0; j < nbr_samples; j++) {
			/* Propose step */
			for (k = 0; k < 3; k++) {
                r1_new[k] = r1_current[k] + Delta * (gsl_rng_uniform(q) - 0.5);
                r2_new[k] = r2_current[k] + Delta * (gsl_rng_uniform(q) - 0.5);
            }

			/* accept proposal? */
			if (rho(r1_new, r2_new, alpha) / rho(r1_current, r2_current, alpha) > gsl_rng_uniform(q)) {
				for (k = 0; k < 3; k++) {
                    r1_current[k] = r1_new[k];
                    r2_current[k] = r2_new[k];
                }

				tmp_nbr_accepted++;
			}

			/* Save relevant quantities */
			E_local_list[j * nbr_walkers + i] = E_local(r1_current, r2_current, alpha);
		}
		nbr_accepted += tmp_nbr_accepted;
	}
    energy_integral = mean(E_local_list, nbr_samples * nbr_walkers); // expected value of energy


	/* Print some info */
	if (talk != 0) {
		printf("\n");
		printf("################ TASK 1 ################\n");
		printf("alpha                   = %f\n", alpha);
		printf("# of walkers            = %d\n", nbr_walkers);
		printf("# of samples per walker = %d\n", nbr_samples);
		printf("accepted ratio          = %.2f\n", (double) nbr_accepted / (nbr_walkers * nbr_samples));
		printf("energy integral         = %f\n", energy_integral);
	}

	/* free memory */
	gsl_rng_free(q);
    free(E_local_list);
}


int main() {
	double alpha = 0.1; // wavefunction ansatz parameter
	double alpha_max = 0.12; // in task 4, we generate initial guesses for alpha_min in this interval
	double alpha_min = 0.175;

    task1(alpha, (int) 1e4, 1);

	task2a(alpha, 1);

	task2b();

	task3(1);

	task4(alpha_max, alpha_min, 1);

	task5(0.142466, (int) 1e7, 1);

	alpha++; // BODGE
    return 0;
}