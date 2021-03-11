/*
 * HPC TMA881 - assignment 4
 * Simon Stefanus Jacobsson 2020-XX-XX
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> /* For getopt */
#include <assert.h>

/* #include <clBLAS.h> */
#include <CL/cl.h>

#define MAX_NBR_ITERATIONS 100000

int main(int argc, char* argv[])
{
    size_t nbr_iterations = 10;
    double diffusion_constant = 0.1;

    /* Set nbr of iterations and diffusion constant */
    int option;
    while ((option = getopt(argc, argv, "n:d:")) != -1) {
	switch (option) {
	case 'n':
		nbr_iterations = atoi(optarg);
		break;
	case 'd':
		diffusion_constant = atoi(optarg);
		break;
	}
    }
    assert(0 < nbr_iterations && nbr_iterations <= MAX_NBR_ITERATIONS);
    assert(0 < diffusion_constant);

    /* Read height and width from 1st line of file */
    FILE* diffusion_file;
    size_t height;
    size_t width;

    diffusion_file = fopen("diffusion", "r");
    if (diffusion_file == NULL) {
	printf("error opening file\n");
	return -1;
    }
    fscanf(diffusion_file, "%zu %zu", &height, &width);

    /* Read initial values from file */
	double **temperatures = malloc(height * sizeof(double *));
	for (size_t row = 0; row < height; row++) {
		temperatures[row] = calloc(width, sizeof(double));
	}

	char line[100];
	while (fgets(line, 100, diffusion_file) != NULL) {
		double temperature;
		size_t row;
		size_t col;

		sscanf(line, "%zu %zu %lf", &row, &col, &temperature);
		temperatures[row][col] = temperature;
	}
    fclose(diffusion_file);


	
	for (size_t t = 0; t < nbr_iterations; t++) {
		temperatures[i, j] += diffusion_constant * ((\
		temperatures[i-1, j] + \
		temperatures[i+1,j] + \
		temperatures[i, j-1} + \
		temperatures[i, j+1]) / 4 - temperatures[i,j])
	}
	



}

