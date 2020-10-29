
double square(double a);
double get_kinetic_energy(double velocities[][3], int velocities_length);
double get_temperature(double velocities[][3], int velocities_length);
double get_pressure(double a0, double positions[][3], double velocities[][3], int positions_length);
void equilibration_update(double *a0, double positions[][3], double velocities[][3], double temperature, double pressure, double T_eq, double P_eq, double T_decay_constant, double P_decay_constant, double timestep);
double mean_squared_displacement(int t_index, double ***positions_list, double timestep, int nbr_timesteps);
double velocity_correlation(int t_index, double ***velocities_list, double timestep, int nbr_timesteps);
