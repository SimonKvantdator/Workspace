#include "utility_funcs.h"

#include <stdio.h>
#include <math.h>
#include "verlet_funcs.h"
#include "alpotential.h"

/* define some shorthands (same as in main) */
#define kB (8.617e-5) // Boltzmann constant [eV / K]
#define Al_MASS 0.0027964394 // [eV ps^2 / A^2]
#define Nc 4
#define NBR_CELLS (Nc * Nc * Nc)
#define NBR_ATOMS (4 * NBR_CELLS)


double square(double a) {
	return a * a;
}


/* Returns total kinetic energy of supercell */
double get_kinetic_energy(double velocities[][3], int velocities_length) {
	/* Declare variable to sum over */
	int i;

	/* Initiate sum of kinetic enrgies */
	double sum = 0.0;

	for (i = 0; i < velocities_length; i++){
		sum += 0.5 * pow(velocities[i][0], 2) * Al_MASS;
		sum += 0.5 * pow(velocities[i][1], 2) * Al_MASS;
		sum += 0.5 * pow(velocities[i][2], 2) * Al_MASS;
	}

	return sum;
}


/* Returns temperature of supercell */
double get_temperature(double velocities[][3], int velocities_length) {
	double average_kinetic_energy;
	double temperature;

	average_kinetic_energy = get_kinetic_energy(velocities, velocities_length) / velocities_length;
	temperature = average_kinetic_energy * 2.0 / (3.0 * kB);
	
	return temperature;
}


/* Returns pressure of supercell */
double get_pressure(double a0, double positions[][3], double velocities[][3], int positions_length) {
	double volume = a0 * a0 * a0 * Nc * Nc * Nc;

	return ((2.0 / 3.0) * get_kinetic_energy(velocities, positions_length) / NBR_ATOMS + get_virial_AL(positions, a0 * Nc, NBR_ATOMS)) / volume;
}


/* Updates a0, positions, & velocities using temperature & pressure to better match T_eq & P_eq */
void equilibration_update(double *a0, double positions[][3], double velocities[][3], double temperature, double pressure, double T_eq, double P_eq, double T_decay_constant, double P_decay_constant, double timestep) {
	double alpha_T;
	double alpha_P;
	double kappa_T = 2.1; // [A^3 / eV] isothermal compressibility of aluminum @ 300 K
	int i; // variable to sum over

	/* Calculate rescaling factors */
	alpha_T = 1.0 + 2.0 * timestep * (T_eq - temperature) / (T_decay_constant * temperature);
	alpha_P = 1.0 - kappa_T * timestep * (P_eq - pressure) / P_decay_constant;
	
	/* Rescale positions & velocitites in order to get closer to P_eq & T_eq */ 
	for (i = 0; i < NBR_ATOMS; i++) {
		velocities[i][0] *= pow(alpha_T, 1 / 2.0);
		velocities[i][1] *= pow(alpha_T, 1 / 2.0);
		velocities[i][2] *= pow(alpha_T, 1 / 2.0);

        positions[i][0] *= pow(alpha_P, 1 / 3.0);
        positions[i][1] *= pow(alpha_P, 1 / 3.0);
        positions[i][2] *= pow(alpha_P, 1 / 3.0);
	}
	*a0 *= pow(alpha_P, 1 / 3.0);
}


/* Returns mean squared displacement at time t of particles in lattice given an array 
	positions_list[nbr_atoms][nbr_dimensions][2 * nbr_timesteps] */
double mean_squared_displacement(int t_index, double ***positions_list, double timestep, int nbr_timesteps) {
	int i, j; // variables to sum over
	double squared_displacements = 0.0; // tmp variable to hold sum

	/* sum up squared displacements over time and particles */
	for (i = 0; i < nbr_timesteps; i++) {
		for (j = 0; j < NBR_ATOMS; j++) {
			squared_displacements += square(positions_list[j][0][i + t_index] - positions_list[j][0][i]);
			squared_displacements += square(positions_list[j][1][i + t_index] - positions_list[j][1][i]);
			squared_displacements += square(positions_list[j][2][i + t_index] - positions_list[j][2][i]);
		}
	}

	return squared_displacements / (double) (NBR_ATOMS * nbr_timesteps); // average by dividing by nbr_atoms & nbr_timesteps
}


double velocity_correlation(int t_index, double ***velocities_list, double timestep, int nbr_timesteps) {
	int i, j; // variables to sum over
	double vel_corr = 0.0; // tmp variable to hold sum

	/* sum up squared displacements over time and particles */
	for (i = 0; i < nbr_timesteps; i++) {
		for (j = 0; j < NBR_ATOMS; j++) {
			vel_corr += velocities_list[j][0][i + t_index] * velocities_list[j][0][i];
			vel_corr += velocities_list[j][1][i + t_index] * velocities_list[j][1][i];
			vel_corr += velocities_list[j][2][i + t_index] * velocities_list[j][2][i];
		}
	}

	return vel_corr / (double) (NBR_ATOMS * nbr_timesteps); // average by dividing by nbr_atoms & nbr_timesteps
}