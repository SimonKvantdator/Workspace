double ipow(double a, int n);
double sum(double *array, int array_length);
double mean(double *array, int array_length);
double variance(double *array, int array_length);
double autocorrelation(double *f, int f_length, int k);
double block_average(int block_size, int block_index, double *array);
double scalar_product(double v1[3], double v2[3] );
double norm(double r[3]);
void unit(double v_unit[3], double v[3]);
double distance(double r1[3], double r2[3]);
double psi(double r1_vec[3], double r2_vec[3], double alpha);
double E_local (double r1_vec[3], double r2_vec[3], double alpha);
double rho( double r1[3], double r2[3], double alpha);