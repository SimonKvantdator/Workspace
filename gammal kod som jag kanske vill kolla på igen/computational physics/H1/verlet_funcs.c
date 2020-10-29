#include "verlet_funcs.h"

#include <math.h>
#include "utility_funcs.h"
#include "alpotential.h"

/* define some shorthands (same as in main) */
#define kB (8.617e-5) // Boltzmann constant [eV / K]
#define Al_MASS 0.0027964394 // [eV ps^2 / A^2]
#define Nc 4
#define NBR_CELLS (Nc * Nc * Nc)
#define NBR_ATOMS (4 * NBR_CELLS)
#define a0_LIST_LENGTH 45


/* Shorthand for initiating velocities & accelerations */
void init_vel_acc(double velocities[][3], double accelerations[][3], double forces[][3]) {
	int i;
	for (i = 0; i < NBR_ATOMS; i++) {
		velocities[i][0] = 0.0;
		velocities[i][1] = 0.0;
		velocities[i][2] = 0.0;
		accelerations[i][0] = forces[i][0] / Al_MASS;
		accelerations[i][1] = forces[i][1] / Al_MASS;
		accelerations[i][2] = forces[i][2] / Al_MASS;
	}
}


/*
	Run Verlet algorithm 1 time.
	Updates positions, velocities, & accelerations using volicities, accelerations, forces, & timestep.
*/
void verlet_single(double positions[][3], double velocities[][3], double accelerations[][3], double forces[][3], double timestep, double a0) {
	int i;
	for (i = 0; i < NBR_ATOMS; i++) {
		/* v(t+dt/2) */
		velocities[i][0] += timestep * 0.5 * accelerations[i][0];
		velocities[i][1] += timestep * 0.5 * accelerations[i][1];
		velocities[i][2] += timestep * 0.5 * accelerations[i][2];

		/* q(t+dt) */
		positions[i][0] += timestep * velocities[i][0];
		positions[i][1] += timestep * velocities[i][1];
		positions[i][2] += timestep * velocities[i][2];
	}

	/* Update forces */
	get_forces_AL(forces, positions, Nc * a0, NBR_ATOMS);

	for (i = 0; i < NBR_ATOMS; i++) {
		/* a(t+dt) */
		accelerations[i][0] = forces[i][0] / Al_MASS;
		accelerations[i][1] = forces[i][1] / Al_MASS;
		accelerations[i][2] = forces[i][2] / Al_MASS;

		/* v(t+dt) */
		velocities[i][0] += timestep * 0.5 * accelerations[i][0];
		velocities[i][1] += timestep * 0.5 * accelerations[i][1];
		velocities[i][2] += timestep * 0.5 * accelerations[i][2];
	}
}


/*
	Run Verlet algorithm nbr_timesteps times.
	Updates positions, velocities, & accelerations using forces & timestep.
*/
void verlet_many(double positions[][3], double velocities[][3], double accelerations[][3], double a0, double timestep, int nbr_timesteps) {
    /* Initiate forces */
    double forces[NBR_ATOMS][3];
    get_forces_AL(forces, positions, Nc * a0, NBR_ATOMS);

    /* Run Verlet algorithm */
	int i;
	for (i = 0; i < nbr_timesteps; i++){
		/* Update positions, velocities, & accelerations using forces */
		verlet_single(positions, velocities, accelerations, forces, timestep, a0);

		/* Update forces */
		get_forces_AL(forces, positions, Nc * a0, NBR_ATOMS);
	}
}


/*
	Run Verlet algorithm nbr_timesteps times.
	Updates positions, velocities, & accelerations using forces & timestep.
    Stores time averages of temperature & pressure in temperature_average & pressure_average.
*/
void verlet_task3(double temperature_average, double pressure_average, double positions[][3], double velocities[][3], double accelerations[][3], double a0, double timestep, int nbr_timesteps) {
    double temperature;
    temperature_average = 0.0;
    double pressure;
    pressure_average = 0.0;
    
    /* Initiate forces */
    double forces[NBR_ATOMS][3];
    get_forces_AL(forces, positions, Nc * a0, NBR_ATOMS);

    /* Run Verlet algorithm */
	int i;
	for (i = 0; i < nbr_timesteps; i++){
		/* Update positions, velocities, & accelerations using forces */
		verlet_single(positions, velocities, accelerations, forces, timestep, a0);

        /* Update temperature & pressure time averages */
        temperature = get_temperature(velocities, NBR_ATOMS);
        pressure = get_pressure(a0, positions, velocities, NBR_ATOMS);
        temperature_average += temperature / (double) nbr_timesteps;
        pressure_average += pressure / nbr_timesteps;
	}
}
