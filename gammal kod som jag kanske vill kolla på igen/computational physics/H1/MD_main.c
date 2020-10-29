#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "initfcc.h"
#include "alpotential.h"
#include "verlet_funcs.h"
#include "utility_funcs.h"
#include "fft_func.h"

/* define some shorthands */
#define kB (8.617e-5) // Boltzmann constant [eV / K]
#define Al_MASS 0.0027964394 // [eV ps^2 / A^2]
#define Nc 4
#define NBR_CELLS (Nc * Nc * Nc)
#define NBR_ATOMS (4 * NBR_CELLS)
#define a0_LIST_LENGTH 45

/* declare some global variables */
double a0;


void task1() {
	double positions[NBR_ATOMS][3]; // [A]
	double a0; // [A]
	double a0_list[a0_LIST_LENGTH];
	double potential_energy; // [eV]

	/*declare variable to sum over */
	int i;

	FILE *potential_energy_file;
	FILE *a0_file;

	/* Assign list of unit cell lengths */
	for (i = 0; i < a0_LIST_LENGTH; i++) {
		a0_list[i] = pow(64.0 + 0.1 * i, 1 / 3.0); // in order to match Fig 1
	}

	potential_energy_file = fopen("potential_energy.txt", "w");
	a0_file = fopen("a0.txt", "w");

	/* Calculate potential energies */
	for (i = 0; i < a0_LIST_LENGTH; i++) {
		a0 = a0_list[i];
		init_fcc(positions, Nc, a0);
		potential_energy = get_energy_AL(positions, Nc * a0, NBR_ATOMS);

		/* write energies to file */
		fprintf(potential_energy_file, "%e\n", potential_energy / NBR_CELLS); // potential energy per unit cell
		fprintf(a0_file, "%e\n", a0);
   }

   fclose(potential_energy_file);
   fclose(a0_file);
}


void task2() {
	double positions [NBR_ATOMS][3]; // [A]
	double velocities [NBR_ATOMS][3]; // [A / ps]
	double accelerations [NBR_ATOMS][3]; // [A / ps^2]
	double forces [NBR_ATOMS][3]; // [eV / A]
	double disturbance; // [A]
	int i, j; // variables to sum over

	/* Initiate some variables */	
	int nbr_timesteps = 1e3;
	double dt = 1e-3; // [ps]
	a0 = pow(66.0, 1 / 3.0); // [A]

	/* Allocate some memory & declare some files for storing result from Verlet algorithm */
	double *potential_energies = malloc(nbr_timesteps * sizeof(double));
	double *kinetic_energies = malloc(nbr_timesteps * sizeof(double));
	FILE *potential_energies_file;
	FILE *kinetic_energies_file;
	FILE *parameters_file;


	/* Initiate positions & introduce disturbances */
	init_fcc(positions, Nc, a0);
	for (i = 0; i < NBR_ATOMS; i++){
		for (j = 0; j < 3; j++) {
			disturbance = 2 * 0.065 * (0.5 - (double) rand() / (double) RAND_MAX) * a0; // +-6.5% of a0 in each direction
			positions[i][j] += disturbance;
		}
	}

	/* Initiate velocities & accelerations */
	get_forces_AL(forces, positions, Nc * a0, NBR_ATOMS);
	init_vel_acc(velocities, accelerations, forces);

	/* Perform velocity Verlet algorithm */
	for (i = 0; i < nbr_timesteps; i++) {
		/* Calculate & store energies */
		potential_energies[i] = get_energy_AL(positions, Nc * a0, NBR_ATOMS);
		kinetic_energies[i] = get_kinetic_energy(velocities, NBR_ATOMS);

		/* Update positions, velocities, & accelerations using forces */
		verlet_single(positions, velocities, accelerations, forces, dt, a0);
	}


	/* write energies to file */
	potential_energies_file = fopen("potential_energies_task2.bin", "w");
	kinetic_energies_file = fopen("kinetic_energies_task2.bin", "w");
	parameters_file = fopen("parameters_task2.bin", "w");

	fwrite(potential_energies, sizeof(double), nbr_timesteps, potential_energies_file);
	fwrite(kinetic_energies, sizeof(double), nbr_timesteps, kinetic_energies_file);
    double paramList[4] = {nbr_timesteps, dt, NBR_ATOMS, a0};
	int nbr_parameters = 4;
    fwrite(paramList, nbr_parameters * sizeof(double), 1, parameters_file);

    fclose(potential_energies_file);
    fclose(kinetic_energies_file);
    fclose(parameters_file);

	free(potential_energies);
	free(kinetic_energies);
}


void task3() {
	double positions[NBR_ATOMS][3]; // [A]
	double velocities [NBR_ATOMS][3]; // [A / ps]
	double accelerations [NBR_ATOMS][3]; // [A / ps^2]
	double forces [NBR_ATOMS][3]; // [eV / A]
	double disturbance; // [A]
	double temperature = 0.0; // [K]
	double pressure = 0.0; // []
	int i, j; // variables to sum over

	/* Initiate some variables */	
	int nbr_timesteps_per_epoch = 1;
	int nbr_epochs = 1e4;
	double dt = 1e-3;
	a0 = pow(66.0, 1 / 3.0);
	double T_eq = 273.15 + 500.0; // [K]
	double P_eq = 6.242e-7; // 1 bar in [eV / A^3]
	double T_decay_constant = 100.0 * dt; // [ps]
	double P_decay_constant = 100.0 * dt; // [ps]

	/* Variables for writing to file */
	double temperatures[nbr_epochs];
	double pressures[nbr_epochs];
	double parameters[6] = {nbr_timesteps_per_epoch, nbr_epochs, T_decay_constant, P_decay_constant, a0, dt};
	int nbr_parameters = 6;
	FILE *temperatures_file;
	FILE *pressures_file;
	FILE *parameters_file;
	FILE *final_positions_file;
	FILE *final_velocities_file;


	/* Initiate positions & introduce disturbances */
	init_fcc(positions, Nc, a0);
	for (i = 0; i < NBR_ATOMS; i++) {
		for (j = 0; j < 3; j++) {
			disturbance = 2 * 0.065 * (0.5 - (double) rand() / (double) RAND_MAX) * a0; // +-6.5% of a0 in each direction
			positions[i][j] += disturbance;
		}
	}

	/* Initiate velocities & accelerations */
	get_forces_AL(forces, positions, Nc * a0, NBR_ATOMS);
	init_vel_acc(velocities, accelerations, forces);

	/* Equilibrate the lattice */
	for (i = 1; i < nbr_epochs + 1; i++) {

		/* Perform one epoch of velocity Verlet algorithm.
		temperature & pressure are calculated from time averages. */
		verlet_task3(temperature, pressure, positions, velocities, accelerations, a0, dt, nbr_timesteps_per_epoch);

		/* Update a0, positions, & velocities */
		temperature = get_temperature(velocities, NBR_ATOMS);
		pressure = get_pressure(a0, positions, velocities, NBR_ATOMS);
		equilibration_update(&a0, positions, velocities, temperature, pressure, T_eq, P_eq, T_decay_constant, P_decay_constant, dt);

		/* Save temperatures & pressures for later */
		temperatures[i-1] = temperature;
		pressures[i-1] = pressure;
	}

	/* Write relevant info to file */
	temperatures_file = fopen("temperatures_task3.bin", "w"); // to be used in plot
	pressures_file = fopen("pressures_task3.bin", "w"); // to be used in plot
	parameters_file = fopen("parameters_task3.bin", "w"); // to be used in plot
	final_positions_file = fopen("final_positions_task3.bin", "w"); // to be used in task 5
	final_velocities_file = fopen("final_velocities_task3.bin", "w"); // to be used in task 5

	fwrite(temperatures, sizeof(double), nbr_epochs, temperatures_file);
	fwrite(pressures, sizeof(double), nbr_epochs, pressures_file);
	fwrite(parameters, nbr_parameters * sizeof(double), 1, parameters_file);
	fwrite(positions, NBR_ATOMS * sizeof(double) * 3, 1, final_positions_file);
	fwrite(velocities, NBR_ATOMS * sizeof(double) * 3, 1, final_velocities_file);

	fclose(temperatures_file);
	fclose(pressures_file);
	fclose(parameters_file);
	fclose(final_positions_file);
	fclose(final_velocities_file);
}


void task4() {
	double positions[NBR_ATOMS][3]; // [A]
	double velocities[NBR_ATOMS][3]; // [A / ps]
	double accelerations[NBR_ATOMS][3]; // [A / ps^2]
	double forces[NBR_ATOMS][3]; // [eV / A]
	double disturbance; // [A]
	double temperature = 0.0; // [K]
	double pressure = 0.0; // []
	int i, j; // variables to sum over

	/* Initiate some variables */	
	int nbr_timesteps_per_epoch = 1;
	int nbr_epochs = 1e4;
	double dt = 1e-3;
	a0 = pow(66.0, 1 / 3.0);
	double T_eq = 273.15 + 700.0; // [K]
	double T_pre_eq = 2000; // [K] pre-equilibration temperature for melting the aluminum
	double P_eq = 6.242e-7; // [eV / A^3]
	double T_decay_constant = 100.0 * dt; // [ps]
	double P_decay_constant = 100.0 * dt; // [ps]

	/* Variables for writing to file */
	double temperatures[nbr_epochs];
	double pressures[nbr_epochs];
	double parameters[6] = {nbr_timesteps_per_epoch, nbr_epochs, T_decay_constant, P_decay_constant, a0, dt};
	FILE *temperatures_file;
	FILE *pressures_file;
	FILE *parameters_file;
	FILE *final_positions_file;
	FILE *final_velocities_file;


	/* Initiate positions & introduce disturbances */
	init_fcc(positions, Nc, a0);
	for (i = 0; i < NBR_ATOMS; i++) {
		for (j = 0; j < 3; j++) {
			disturbance = 2 * 0.065 * (0.5 - (double) rand() / (double) RAND_MAX) * a0; // +-6.5% of a0 in each direction
			positions[i][j] += disturbance;
		}
	}

	/* Initiate velocities & accelerations */
	get_forces_AL(forces, positions, Nc * a0, NBR_ATOMS);
	init_vel_acc(velocities, accelerations, forces);

	/* Pre-equilibrate the lattice temperature so that we're certian we're in liquid phase */
	for (i = 0; i < nbr_epochs / 10; i++) {

		/* Perform one epoch of velocity Verlet algorithm.
		temperature & pressure are calculated from time averages. */
		verlet_task3(temperature, pressure, positions, velocities, accelerations, a0, dt, nbr_timesteps_per_epoch);

		/* Update a0, positions, & velocities */
		temperature = get_temperature(velocities, NBR_ATOMS);
		pressure = get_pressure(a0, positions, velocities, NBR_ATOMS);
		equilibration_update(&a0, positions, velocities, temperature, pressure, T_pre_eq, P_eq, T_decay_constant, P_decay_constant, dt);
	}


	/* Equilibrate the lattice */
	for (i = 0; i < nbr_epochs; i++) {

		/* Perform one epoch of velocity Verlet algorithm.
		temperature & pressure are calculated from time averages. */
		verlet_task3(temperature, pressure, positions, velocities, accelerations, a0, dt, nbr_timesteps_per_epoch);

		/* Update a0, positions, & velocities */
		temperature = get_temperature(velocities, NBR_ATOMS);
		pressure = get_pressure(a0, positions, velocities, NBR_ATOMS);
		equilibration_update(&a0, positions, velocities, temperature, pressure, T_eq, P_eq, T_decay_constant, P_decay_constant, dt);

		/* Save temperatures & pressures for later */
		temperatures[i] = temperature;
		pressures[i] = pressure;
	}

	/* Write relevant info to file */
	temperatures_file = fopen("temperatures_task4.bin", "w"); // to be used in plot
	pressures_file = fopen("pressures_task4.bin", "w"); // to be used in plot
	parameters_file = fopen("parameters_task4.bin", "w"); // to be used in plot
	final_positions_file = fopen("final_positions_task4.bin", "w"); // to be used in task 5
	final_velocities_file = fopen("final_velocities_task4.bin", "w"); // to be used in task 5

	fwrite(temperatures, sizeof(double), nbr_epochs, temperatures_file);
	fwrite(pressures, sizeof(double), nbr_epochs, pressures_file);
	fwrite(parameters, sizeof(parameters), 1, parameters_file);
	fwrite(positions, NBR_ATOMS * sizeof(double) * 3, 1, final_positions_file);
	fwrite(velocities, NBR_ATOMS * sizeof(double) * 3, 1, final_velocities_file);

	fclose(temperatures_file);
	fclose(pressures_file);
	fclose(parameters_file);
	fclose(final_positions_file);
	fclose(final_velocities_file);
}


void task5() {
	int nbr_timesteps = 1000;
	double dt = 1e-3; // [ps]
	double parameters[2] = {dt, nbr_timesteps};
	int nbr_parameters = 2;
	int i, j;

	double positions[NBR_ATOMS][3]; // [A]
	double velocities[NBR_ATOMS][3]; // [A / ps]
	double accelerations[NBR_ATOMS][3]; // [A / ps^2]
	double forces[NBR_ATOMS][3]; // [eV / A]
	double initial_parameters[6];

	/* Allocate some memory for storing in each timestep */
	double *mean_squared_displacements = malloc(nbr_timesteps * sizeof(double));
	double ***positions_list = (double ***)malloc(NBR_ATOMS * sizeof(double **)); // positions_list[nbr_atoms][3][2 * nbr_timesteps]
    for (i = 0; i < NBR_ATOMS; i++) {
        positions_list[i] = (double **)malloc(3 * sizeof(double *));
		positions_list[i][0] = (double *)malloc(2 * nbr_timesteps * sizeof(double));
		positions_list[i][1] = (double *)malloc(2 * nbr_timesteps * sizeof(double));
		positions_list[i][2] = (double *)malloc(2 * nbr_timesteps * sizeof(double));
	}

	FILE *initial_positions_file;
	FILE *initial_velocities_file;
	FILE *initial_parameters_file;
	FILE *parameters_file;
	FILE *mean_squared_displacements_file;

	/* Initiate positions & velocities */
	/* The final positions & velocities from Task 3/4 are tuned to have temperature 500/700 C & pressure 1 bar */
	initial_positions_file = fopen("final_positions_task4.bin", "r");
	initial_velocities_file = fopen("final_velocities_task4.bin", "r");
	initial_parameters_file = fopen("parameters_task4.bin", "r");

	int tmp; // this is a bodgy way of suppressing warning about fread:s return value unused
	tmp = fread(positions, NBR_ATOMS * sizeof(double) * 3, 1, initial_positions_file);
	tmp = fread(velocities, NBR_ATOMS * sizeof(double) * 3, 1, initial_velocities_file);
	tmp = fread(initial_parameters, nbr_parameters * sizeof(double), 1, initial_parameters_file);
	tmp++;

	fclose(initial_positions_file);
	fclose(initial_velocities_file);
	fclose(initial_parameters_file);


	/* Initiate forces & accelerations */
	a0 = initial_parameters[4];
	get_forces_AL(forces, positions, Nc * a0, NBR_ATOMS);
	for (i = 0; i < NBR_ATOMS; i++) {
		accelerations[i][0] = forces[i][0] / Al_MASS;
		accelerations[i][1] = forces[i][1] / Al_MASS;
		accelerations[i][2] = forces[i][2] / Al_MASS;
	}

	/* Perform velocity Verlet algorithm */
	for (i = 1; i < 2 * nbr_timesteps; i++) {
		/* Update positions, velocities, & accelerations using forces */
		verlet_single(positions, velocities, accelerations, forces, dt, a0);

		/* Store positions */
		for (j = 0; j < NBR_ATOMS; j++) {
			positions_list[j][0][i] = positions[j][0];
			positions_list[j][1][i] = positions[j][1];
			positions_list[j][2][i] = positions[j][2];
		}
	}

	/* Calculate mean squared displacements */
	for (i = 0; i < nbr_timesteps; i++) {
		mean_squared_displacements[i] = mean_squared_displacement(i, positions_list, dt, nbr_timesteps);
	}
	
	/* Write mean squared displacement to file */
	mean_squared_displacements_file = fopen("mean_squared_displacements_task5.bin", "w");
	parameters_file = fopen("parameters_task5.bin", "w");

	fwrite(mean_squared_displacements, nbr_timesteps * sizeof(double), 1, mean_squared_displacements_file);
	fwrite(parameters, nbr_parameters * sizeof(double), 1, parameters_file);
	
	fclose(mean_squared_displacements_file);
	fclose(parameters_file);

	free(mean_squared_displacements);
	free(positions_list);
}


void task6() {
	int nbr_timesteps = 1000;
	double dt = 1e-3; // [ps]
	double parameters[2] = {dt, nbr_timesteps};
	int i, j;

	double positions[NBR_ATOMS][3]; // [A]
	double velocities[NBR_ATOMS][3]; // [A / ps]
	double accelerations[NBR_ATOMS][3]; // [A / ps^2]
	double forces[NBR_ATOMS][3]; // [eV / A]
	double initial_parameters[6];
	int nbr_initial_parameters = 6;
	int nbr_parameters = 6;

	/* Allocate some memory for storing in each timestep */
	double *velocity_correlations = malloc(nbr_timesteps * sizeof(double));
	double ***velocities_list = (double ***)malloc(NBR_ATOMS * sizeof(double **)); // velocities_list[nbr_atoms][3][2 * nbr_timesteps]
    for (i = 0; i < NBR_ATOMS; i++) {
        velocities_list[i] = (double **)malloc(3 * sizeof(double *));
		velocities_list[i][0] = (double *)malloc(2 * nbr_timesteps * sizeof(double));
		velocities_list[i][1] = (double *)malloc(2 * nbr_timesteps * sizeof(double));
		velocities_list[i][2] = (double *)malloc(2 * nbr_timesteps * sizeof(double));
	}

	FILE *initial_positions_file;
	FILE *initial_velocities_file;
	FILE *initial_parameters_file;
	FILE *velocities_file;
	FILE *velocity_correlation_file;
	FILE *parameters_file;

	/* Initiate positions & velocities */
	/* The final positions & velocities from Task 3/4 are tuned to have temperature 500/700 C & pressure 1 bar */
	initial_positions_file = fopen("final_positions_task3.bin", "r");
	initial_velocities_file = fopen("final_velocities_task3.bin", "r");
	initial_parameters_file = fopen("parameters_task3.bin", "r");

	int tmp; // this is a bodgy way of suppressing warning about fread:s return value unused
	tmp = fread(positions, NBR_ATOMS * sizeof(double) * 3, 1, initial_positions_file);
	tmp = fread(velocities, NBR_ATOMS * sizeof(double) * 3, 1, initial_velocities_file);
	tmp = fread(initial_parameters, nbr_initial_parameters * sizeof(double), 1, initial_parameters_file);
	tmp++;

	fclose(initial_positions_file);
	fclose(initial_velocities_file);
	fclose(initial_parameters_file);


	/* Initiate forces & accelerations */
	a0 = initial_parameters[4];
	get_forces_AL(forces, positions, Nc * a0, NBR_ATOMS);
	for (i = 0; i < NBR_ATOMS; i++) {
		accelerations[i][0] = forces[i][0] / Al_MASS;
		accelerations[i][1] = forces[i][1] / Al_MASS;
		accelerations[i][2] = forces[i][2] / Al_MASS;
	}

	/* Perform velocity Verlet algorithm */
	for (i = 1; i < 2 * nbr_timesteps; i++) {
		/* Update positions, velocities, & accelerations using forces */
		verlet_single(positions, velocities, accelerations, forces, dt, a0);

		/* Store velocities */
		for (j = 0; j < NBR_ATOMS; j++) {
			velocities_list[j][0][i] = velocities[j][0];
			velocities_list[j][1][i] = velocities[j][1];
			velocities_list[j][2][i] = velocities[j][2];
		}
	}

	/* Calculate velocity_correlation */
	for (i = 0; i < nbr_timesteps; i++) {
		velocity_correlations[i] = velocity_correlation(i, velocities_list, dt, nbr_timesteps);
	}
	
	/* Write velocity correlation to file */
	velocities_file = fopen("velocities_task6.bin", "w");
	velocity_correlation_file = fopen("velocity_correlations_task6.bin", "w");
	parameters_file = fopen("parameters_task6.bin", "w");

	for (i = 0; i < NBR_ATOMS; i++) { // we have to write each list by itself to file
		for (j = 0; j < 3; j++) {
			fwrite(velocities_list[i][j], nbr_timesteps * 2 * sizeof(double), 1, velocities_file);
		}
	}
	fwrite(velocity_correlations, nbr_timesteps * sizeof(double), 1, velocity_correlation_file);
	fwrite(parameters, nbr_parameters * sizeof(double), 1, parameters_file);
	
	fclose(velocities_file);
	fclose(velocity_correlation_file);
	fclose(parameters_file);

	free(velocity_correlations);
	free(velocities_list);
}


void task7() {
	int nbr_timesteps = 1000; // ought to be the same as in task 6 (BODGE)
	double dt = 1e-3; // [ps] ought to be the same as in task 6 (BODGE)
	// double parameters[2] = {dt, nbr_timesteps};
	int i, j, k; // for summing over

	/* Allocate some memory for storing in each timestep */
	// double *velocity_correlations = malloc(nbr_timesteps * sizeof(double));
	double ***velocities_list = (double ***)malloc(NBR_ATOMS * sizeof(double **)); // velocities_list[nbr_atoms][3][2 * nbr_timesteps]
    for (i = 0; i < NBR_ATOMS; i++) {
        velocities_list[i] = (double **)malloc(3 * sizeof(double *));
		velocities_list[i][0] = (double *)malloc(2 * nbr_timesteps * sizeof(double));
		velocities_list[i][1] = (double *)malloc(2 * nbr_timesteps * sizeof(double));
		velocities_list[i][2] = (double *)malloc(2 * nbr_timesteps * sizeof(double));
	}

	/* arrays for FFT */ 
	double *tmp_array = malloc(nbr_timesteps * sizeof(double));
	double *powspec = malloc(nbr_timesteps * sizeof(double));
	double *total_powspec = malloc(nbr_timesteps * sizeof(double));
	for(k = 0; k < NBR_ATOMS; k++) { // initiate total_powspec
		total_powspec[k] = 0.0;
	}

	double *freq = malloc(nbr_timesteps * sizeof(double));
 

	FILE *velocities_file;
	FILE *powerspectrum_file;
	FILE *frequencies_file;

	/* Read velocities from task 6 from file */
	velocities_file = fopen("velocities_task6.bin", "r");
	int tmp;
	for (i = 0; i < NBR_ATOMS; i++) {
		for (j = 0; j < 3; j++) {
			tmp = fread(velocities_list[i][j], nbr_timesteps * 2 * sizeof(double), 1, velocities_file);
		}
	}
	tmp++;
	fclose(velocities_file);

	/* make FFT (powerspectrum) */
	for(i = 0; i < NBR_ATOMS; i++) {
		for(j = 0; j < 3; j++) {
			tmp_array = velocities_list[i][j];
			powerspectrum(tmp_array, powspec, nbr_timesteps); // fft is same as inverse fft here since our function is even
			for(k = 0; k < NBR_ATOMS; k++) {
				total_powspec[k] += powspec[k] / NBR_ATOMS;
			}
		}
	}
	// powerspectrum_shift(total_powspec, nbr_timesteps);	
	fft_freq(freq, dt, nbr_timesteps);
	// fft_freq_shift(freq, dt, nbr_timesteps);


	/* save powerspectrum for plotting */
	powerspectrum_file = fopen("powerspectrum_task7.bin", "w");
	frequencies_file = fopen("frequencies_task7.bin", "w");

	fwrite(total_powspec, nbr_timesteps * sizeof(double), 1, powerspectrum_file);
	fwrite(freq, nbr_timesteps * sizeof(double), 1, frequencies_file);
}


/* Main program */
int main()
{
	/* Initiate RNG */
	srand(time(NULL));

	// task1();

	// task2();

	// task3();

	// task4();

	// task5();

	// task6();

	task7();

   return 0;
}





