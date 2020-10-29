double square(double a);
double sum(double *array, int array_length);
double mean(double *array, int array_length);
double variance(double *array, int array_length);

double integrand_task1(double x);

double f_task2(double x);
double F_inv_task2(double x);
double integrand_task2(double x);

double f_task3(double x, double y, double z);
double integrand_task3(double x, double y, double z);

double autocorrelation(double *f, int f_length, int k);
double Fj(int j, int B, double *f); // BODGE
double block_average(int block_size, int block_index, double *array);
