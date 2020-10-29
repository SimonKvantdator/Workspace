

void init_vel_acc(double velocities[][3], double accelerations[][3], double forces[][3]);
void verlet_single(double positions[][3], double velocities[][3], double accelerations[][3], double forces[][3], double timestep, double a0);
void verlet_many(double positions[][3], double velocities[][3], double accelerations[][3], double a0, double timestep, int nbr_timesteps);
void verlet_task3(double T_average, double P_average, double positions[][3], double velocities[][3], double accelerations[][3], double a0, double timestep, int nbr_timesteps);
