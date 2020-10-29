/*
E1_main.c
 
Created by AL on 2013-10-24.
Further developed by Martin Gren on 2014-10-20.
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "../Task5/func.h"
#include "../Task4/fft_func.h"
#define PI 3.141592653589
#define nbr_of_timesteps 32767 /* nbr_of_timesteps+1 = power of 2, for best speed */
#define nbr_of_particles 3 /* The number of particles is 3 */

/* Main program */
int main()
{
	/* Declartion of variables */
	double timestep;
	int i,j;
	double timestep_sq,current_time;
	double m;
	double kappa;
	double freq[nbr_of_timesteps];
	
	/* declare file variables */
	FILE *displacements_file;
	FILE *energies_file;
	FILE *powerspectrums_file;

	/* displacement, velocity and acceleration */
	double q[nbr_of_particles];
	double v[nbr_of_particles];
	double a[nbr_of_particles];

	/* potential, kinetic, & total energy */
	double potential_energy;
	double kinetic_energy;
	double total_energy;


	/* Allocating memory for large vectors */
	/* displacements for writing to file */
	double *q_1 = malloc((nbr_of_timesteps+1) * sizeof (double));
	double *q_2 = malloc((nbr_of_timesteps+1) * sizeof (double));
	double *q_3 = malloc((nbr_of_timesteps+1) * sizeof (double));

	/* energies for writing to file */
	double *potential_energies = malloc((nbr_of_timesteps+1) * sizeof (double));
	double *kinetic_energies = malloc((nbr_of_timesteps+1) * sizeof (double));
	double *total_energies = malloc((nbr_of_timesteps+1) * sizeof (double));

	/* powerspectrum for writing to file */
	double *powerspectrum_q1 = malloc((nbr_of_timesteps+1) * sizeof (double));
	double *powerspectrum_q2 = malloc((nbr_of_timesteps+1) * sizeof (double));
	double *powerspectrum_q3 = malloc((nbr_of_timesteps+1) * sizeof (double));


	/* Set variables */
	timestep = 0.01;
	m = 1.0;
	kappa = 1.0;
	timestep_sq = timestep * timestep;


	
	/* Initial conditions */
	/* Set initial displacements and velocites */
	q[0] = 0.1;
	v[0] = 0;

	for (i = 1; i < nbr_of_particles; i++) {
		q[i] = 0;
		v[i] = 0;
	}
	q_1[0] = q[0];
	q_2[0] = q[1];
	q_3[0] = q[2];

	/* Calculate initial accelerations based on initial displacements */
	calc_acc(a, q, m, kappa, nbr_of_particles);

	/* timesteps according to velocity Verlet algorithm */
	for (i = 1; i < nbr_of_timesteps + 1; i++) {
		/* v(t+dt/2) */
		for (j = 0; j < nbr_of_particles; j++) {
		    v[j] += timestep * 0.5 * a[j];
		} 

		/* q(t+dt) */
		for (j = 0; j < nbr_of_particles; j++) {
		    q[j] += timestep * v[j];
		}

		/* a(t+dt) */
		calc_acc(a, q, m, kappa, nbr_of_particles);

		/* v(t+dt) */
		for (j = 0; j < nbr_of_particles; j++) {
		    v[j] += timestep * 0.5 * a[j];
		} 

		/*calculate potential, kinetic, & total energy*/
		potential_energy = calc_pe(q, kappa, nbr_of_particles);
		kinetic_energy = calc_ke(v, nbr_of_particles, m);
		total_energy = potential_energy + kinetic_energy;

		/* Save the energies of the three atoms */
		potential_energies[i] = potential_energy;
		kinetic_energies[i] = kinetic_energy;
		total_energies[i] = total_energy;

		/* Save the displacement of the three atoms */
		q_1[i] = q[0];
		q_2[i] = q[1];
		q_3[i] = q[2];
	}

	/* Print displacement data to output file */
	displacements_file = fopen("displacements.dat","w");
	for (i = 0; i < nbr_of_timesteps + 1; i++) {
		current_time = i * timestep;
		fprintf(displacements_file, "%e \t %e \t %e \t %e\n", current_time, q_1[i], q_2[i], q_3[i]);	
	}

	/* Print energies to file */
	energies_file = fopen("energies.dat", "w");
	for (i = 0; i < nbr_of_timesteps + 1; i++) {
		current_time = i * timestep;
		fprintf(energies_file, "%e \t %e \t %e \t %e\n", current_time, potential_energies[i], kinetic_energies[i], total_energies[i] );	
	}

	/* Calculate power spectrum*/
	powerspectrum(q_1, powerspectrum_q1, nbr_of_timesteps); //nbr_of_timesteps + 1?
	powerspectrum(q_2, powerspectrum_q2, nbr_of_timesteps);
	powerspectrum(q_3, powerspectrum_q3, nbr_of_timesteps);
	powerspectrum_shift(powerspectrum_q1, nbr_of_timesteps);
	powerspectrum_shift(powerspectrum_q2, nbr_of_timesteps);
	powerspectrum_shift(powerspectrum_q3, nbr_of_timesteps);
	fft_freq_shift(freq, timestep, nbr_of_timesteps);

	/* Print powerspectrum data to output file */
	powerspectrums_file = fopen("powerspectrums.dat","w");
	for (i = 0; i < nbr_of_timesteps; i++)	{
		fprintf (powerspectrums_file, "%e \t %e \t %e \t %e\n", freq[i], powerspectrum_q1[i], powerspectrum_q2[i], powerspectrum_q3[i]);
	}


	fclose(displacements_file);
	fclose(energies_file);
	fclose(powerspectrums_file);

	/* Free allocated memory */ 
	free(q_1); q_1 = NULL;
	free(q_2); q_2 = NULL;
	free(q_3); q_3 = NULL;
	
	free(kinetic_energies); kinetic_energies = NULL;
	free(potential_energies); potential_energies = NULL;
	free(total_energies); total_energies = NULL;

	free(powerspectrum_q1); powerspectrum_q1 = NULL;
	free(powerspectrum_q2); powerspectrum_q2 = NULL;
	free(powerspectrum_q3); powerspectrum_q3 = NULL;


	return 0;    
}
