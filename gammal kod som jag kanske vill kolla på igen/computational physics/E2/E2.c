/*
 E2.c
 
 Created by Anders Lindman on 2014-11-04.
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "func.h"
#include "fft_func.h"
#define PI 3.141592653589

/* Main program */
void main()
{
    /*Declare some variables */
    int i, j, k; // summation variables
    double current_time;
    double potential_energy;
    double kinetic_energy;
    double total_energy;

    /* Initiate parameters for Verlet algorithm */
    double dt = 0.1;
    double t_max = 2500.0;
    int nbr_timesteps = (int) t_max / dt + 1;
    printf("nbr_timesteps = %d\n", nbr_timesteps);
    int nbr_particles = 32 + 2; // treating each wall as a particle
    double m = 1.0;
    double kappa = 1.0;
    double alpha = 0.0;
    double P1 = sqrt(2 * nbr_particles); // initial conditions for task2

    /* Allocate some memory for the Verlet algorithm */
    double *q_current = malloc(nbr_particles * sizeof(double));
    double *v_current = malloc(nbr_particles * sizeof(double));
    double *a_current = malloc(nbr_particles * sizeof(double));
    double **q_array = malloc(nbr_timesteps * sizeof(double *)); 
    double **v_array = malloc(nbr_timesteps * sizeof(double *)); 
    for (i = 0; i < nbr_timesteps; i++) {
        q_array[i] = malloc(nbr_particles * sizeof(double));
        v_array[i] = malloc(nbr_particles * sizeof(double));
    }

    /* Allocate some memory for Fourier transform & energies */
    double **powerspectrums = malloc((nbr_timesteps + 1) * sizeof(double *));
    for (i = 0; i < nbr_timesteps; i++) {
        powerspectrums[i] = malloc(nbr_particles * sizeof(double));
    }
    double *freq = malloc(nbr_timesteps * sizeof(double));
    double *potential_energies = malloc(nbr_timesteps * sizeof(double));
    double *kinetic_energies = malloc(nbr_timesteps * sizeof(double));
    double *total_energies = malloc(nbr_timesteps * sizeof(double));

    /* Declare som variables & allocate some memory for energy distribution over modes */
    double factor;
    double **p2P_matrix = malloc(nbr_particles * sizeof(double *));
    double **q2Q_matrix = malloc(nbr_particles * sizeof(double *));
    for (i = 0; i < nbr_particles; i++) {
        p2P_matrix[i] =  malloc(nbr_particles * sizeof(double));
        q2Q_matrix[i] = malloc(nbr_particles * sizeof(double));
    }
    double **mode_energies = malloc(nbr_timesteps * sizeof(double *));
    for (i = 0; i < nbr_timesteps; i++) {
        mode_energies[i] = malloc(nbr_particles * sizeof(double));
    }

    double *Q = malloc(nbr_particles * sizeof(double));
    double *P = malloc(nbr_particles * sizeof(double));
    double *w = malloc(nbr_particles * sizeof(double));

    /* Calculate transformation matrix between p/q & P/Q */    
    factor = 1 / ((double) nbr_particles + 1);
    for (j = 0; j < nbr_particles; j++) {
        for (k = 0; k < nbr_particles; k++) {
            p2P_matrix[j][k] = sqrt(2 * factor) * sin((k + 1) * (j + 1) * PI * factor) / sqrt(m);
            q2Q_matrix[j][k] = sqrt(2 * factor) * sin((k + 1) * (j + 1) * PI * factor) * sqrt(m);
        }
    }

    /* declare file variables */
	FILE *displacements_file;
	FILE *energies_file;
	FILE *mode_energies_file;
	FILE *powerspectrums_file;


    /* Initial conditions */
    for (i = 0; i < nbr_particles; i++) {
        q_current[i] = 0.0;
        v_current[i] = sqrt(2 * factor) * sqrt(m) * P1 * sin(i * 1 * PI * factor) / m;
        a_current[i] = 0.0;
    }

    /* Perform velocity Verlet algorithm */
	for (i = 1; i < nbr_timesteps; i++) {
		/* v(t+dt/2) */
		for (j = 0; j < nbr_particles; j++) {
		    v_current[j] += dt * 0.5 * a_current[j];
		}

		/* q(t+dt) */
		for (j = 0; j < nbr_particles; j++) {
		    q_current[j] += dt * v_current[j];
		}

		/* a(t+dt) */
		calc_acc(a_current, q_current, m, kappa, alpha, nbr_particles);

		/* v(t+dt) */
		for (j = 0; j < nbr_particles; j++) {
		    v_current[j] += dt * 0.5 * a_current[j];
		}

    	/*calculate potential, kinetic, & total energy*/
		potential_energy = calc_pe(q_current, kappa, alpha, nbr_particles);
		kinetic_energy = calc_ke(v_current, nbr_particles, m);
		total_energy = potential_energy + kinetic_energy;

        /* Calculate normal mode energies */
        for (j = 0; j < nbr_particles; j++){
            Q[j] = 0;
            P[j] = 0;
            for (k = 0; k < nbr_particles; k++){
                Q[j] += q_current[k] * q2Q_matrix[j][k];
                P[j] += m * v_current[k] * p2P_matrix[j][k];
            }

            w[j] = 2 * sqrt(kappa / m) * sin(j * PI * factor / 2); // eigenfrequencies for each mode
            mode_energies[i][j] = (pow(P[j], 2) + pow(w[j], 2) * pow(Q[j], 2)) / 2; // energy for each mode
        }

		/* Save the energies of the atoms */
		potential_energies[i] = potential_energy;
		kinetic_energies[i] = kinetic_energy;
		total_energies[i] = total_energy;

		/* Save the displacement of the atoms */
        for (j = 0; j < nbr_particles; j++){
            q_array[i][j] = q_current[j]; // q_array[j] SHOULD GIVE YOU THE j:TH PARTICLE'S TRAJECTORY
        }
	}

    /* Print displacement data to output file */
	displacements_file = fopen("displacements.dat", "w");
	for (i = 0; i < nbr_timesteps; i++) {
		fprintf(displacements_file, "%e \t", i * dt); // current time
        for(j = 0; j < nbr_particles; j++){
    		fprintf(displacements_file, "%e \t", q_array[i][j]);
        }
        fprintf(displacements_file, "\n");
	}

	/* Print energies to file */
	energies_file = fopen("energies.dat", "w");
	for (i = 0; i < nbr_timesteps; i++) {
		fprintf(energies_file, "%e \t %e \t %e \t %e\n", i * dt, potential_energies[i], kinetic_energies[i], total_energies[i]);	
	}    

    /* Print mode energies to file */
    mode_energies_file = fopen("mode_energies.dat", "w");
	for (i = 0; i < nbr_timesteps; i++)	{
        fprintf(mode_energies_file, "%e\t", i * dt); // print time
        for (j = 0; j < nbr_particles; j++){
            fprintf(mode_energies_file, "%e \t", mode_energies[i][j]);
        }
        fprintf(mode_energies_file, "\n");
	}

	/* Calculate power spectrum*/
    for (i = 0; i < nbr_particles; i++) {
        powerspectrum(q_array[i], powerspectrums[i], nbr_timesteps);
    	powerspectrum_shift(powerspectrums[i], nbr_timesteps);
    }
	fft_freq_shift(freq, dt, nbr_timesteps);

	/* Print powerspectrum data to file */
	powerspectrums_file = fopen("powerspectrums.dat", "w");
	for (i = 0; i < nbr_timesteps; i++)	{
        fprintf(powerspectrums_file, "%e\t", freq[i]);
        for (j = 0; j < nbr_particles; j++){
            fprintf(powerspectrums_file, "%e \t", powerspectrums[i][j]);
        }
        fprintf(powerspectrums_file, "\n");
	}


	fclose(displacements_file);
	fclose(energies_file);
    fclose(mode_energies_file);
	fclose(powerspectrums_file);

	/* Free allocated memory */
    free(q_current);
	free(q_array);
	
	free(kinetic_energies);
	free(potential_energies);
	free(total_energies);

	free(powerspectrums);
	free(freq);
}
