/*
 * HPC TMA881 - assignment 3
 * Simon Stefanus Jacobsson 2020-XX-XX
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>  /* For getopt */
#include <assert.h>
#include <string.h>
#include <threads.h>

#include <math.h>
#include <complex.h>
#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))

#define MAX_d 10
#define MAX_ARRAY_SIZE 100000 //TODO: assert this?
#define NBR_CHARS_PER_LINE 6 /* "R G B\n" */
#define NBR_ITERATIONS_CAP 99

char *attractor_colors[MAX_d + 2];
#define FILL_ATTRACTOR_COLORS(X) \
	X[0] = "2 0 3\n"; \
X[1] = "2 1 4\n"; \
X[2] = "2 2 5\n"; \
X[3] = "2 3 5\n"; \
X[4] = "1 4 5\n"; \
X[5] = "1 4 5\n"; \
X[6] = "1 5 5\n"; \
X[7] = "1 6 5\n"; \
X[8] = "2 7 4\n"; \
X[9] = "3 7 3\n"; \
X[10] = "5 8 2\n"; \
X[11] = "7 8 1\n"; \

char *convergence_colors[10];
#define FILL_CONVERGENCE_COLORS(X) \
	X[0] = "0 0 0\n"; \
X[1] = "1 1 1\n"; \
X[2] = "2 2 2\n"; \
X[3] = "3 3 3\n"; \
X[4] = "4 4 4\n"; \
X[5] = "5 5 5\n"; \
X[6] = "6 6 6\n"; \
X[7] = "7 7 7\n"; \
X[8] = "8 8 8\n"; \
X[9] = "9 9 9\n";

size_t nbr_threads = 8; /* Nbr of threads to run newton iterations with. Initilized now but set later by command line arguments */
size_t size = 100;  /* Nbr of rows and columns in the output. Initilized now but set later by command line arguments */
int d = 7; /* Degree of polynomial. Initilized now but set later by command line arguments */
complex double exact_roots[MAX_d + 2]; /* {1, e^(2pi/d), e^(4pi/d), ..., e^((d-1)pi/d), 0, inf} */
FILE *attractors_file; /* File to write which roots we converge to */
FILE *convergence_file; /* File to write how many iterations before we converge to */

typedef struct {
	size_t initial_row;
	size_t final_row;
	size_t preamble_length;
	mtx_t *mutex;
} main_thread_info_t;

typedef struct {
	size_t initial_row;
	size_t final_row;
	char *attractors_string;
	char *convergence_string;
	size_t preamble_length;
	mtx_t *mutex;
} write_to_file_thread_info_t;

int main_thread(void *args);
int write_to_file_thread(void *args);
int newtons_method(size_t *nbr_iterations, size_t *root_index, double complex z);
complex double my_pow(complex double z, int n);
double norm_squared(complex double z);
inline size_t nbr_digits(size_t n);


int main(int argc, char *argv[]) {

	/* Set nbr of threads and array size */
	int option;
	while((option = getopt(argc, argv, "t:l:")) != -1) {
		switch(option) {
			case 't':
				nbr_threads = atoi(optarg);
				break;
			case 'l':
				size = atoi(optarg);
				break;
		}
	}
	assert(nbr_threads <= size);

	/* Set d and compute the exact roots of z^d - 1 */
	if(argc > 1) {
		d = atoi(argv[argc - 1]); /* d is always the last argument */
		assert(0 < d && d <= MAX_d);
	}
	for(size_t i = 0; i < d; i ++) {
		exact_roots[i] = cexp((complex double) i * 2 * M_PI * I / (complex double) d);
	}
	exact_roots[d] = 0; /* 0 also gets to be a root since it's a stationary point of z -> z - f(z) / f'(c) */

	/* Each attractor gets represented by a color and each bin of nbr of iterations gets represented by a shade */
	FILL_ATTRACTOR_COLORS(attractor_colors);
	FILL_CONVERGENCE_COLORS(convergence_colors);


	/* Open ppm files for writing results to */
	char attractors_filename[26];
	char convergence_filename[26];
	sprintf(attractors_filename, "newton_attractors_x%i.ppm", d);
	sprintf(convergence_filename, "newton_convergence_x%i.ppm", d);
	attractors_file = fopen(attractors_filename, "w");
	convergence_file = fopen(convergence_filename, "w");

	if(attractors_file == NULL || convergence_file == NULL) {
		printf("error opening file\n");
		return -1;
	}


	/* Fill preamble of ppm file with
	 * P3
	 * L L
	 * M
	 * where L is the number of rows and columns and M is the maximal color value */
	char preamble[20];
	sprintf(preamble, "P3\n%zu %zu\n%i\n", size, size, 9);
	size_t preamble_length = strlen(preamble);
	fwrite(preamble, sizeof(char), preamble_length, attractors_file);
	fwrite(preamble, sizeof(char), preamble_length, convergence_file);


	/* Fill body of ppm file */
	thrd_t threads[nbr_threads];
	main_thread_info_t threads_info[nbr_threads];
	mtx_t mutex;
	mtx_init(&mutex, mtx_plain);
	//TODO: create thread that writes to file
	//TODO: how to communicate with that thread?

	/* Each thread gets a nbr of rows to fill out the strings with */
	size_t initial_row = 0;
	size_t final_row;
	for (size_t i = 0; i < nbr_threads; i++) {
		/* Divide up size into nbr_threads blocks as equally as possible */
		size_t final_row = (i < size % nbr_threads) ? \
						   MIN(initial_row + (size / nbr_threads) + 1, size) : \
						   MIN(initial_row + (size / nbr_threads), size);

		threads_info[i].initial_row = initial_row;
		threads_info[i].final_row = final_row;
		threads_info[i].preamble_length = preamble_length;
		threads_info[i].mutex = &mutex;

		int r;
		r = thrd_create(&threads[i], main_thread, (void*) &threads_info[i]);
		if(r != thrd_success) {
			fprintf(stderr, "Couldn't create thread\n");
			exit(1);
		}

		initial_row = final_row;
	}

	/* Make sure all other threads have finished */
	for (size_t i = 0; i < nbr_threads; i++) {
		int thread_return;
		thrd_join(threads[i], &thread_return);
	}
	mtx_destroy(&mutex);

	fclose(attractors_file);
	fclose(convergence_file);


	return 0;
}


/* Fills out lines initial_row to final_row in attractors_string and convergence_string with the RGB values corresponding to which attractor was approached and how many iterations it took */
int main_thread(void *args) {
	/* Read arguments */
	const main_thread_info_t *thread_info = (main_thread_info_t*) args;
	const size_t initial_row = thread_info->initial_row;
	const size_t final_row = thread_info->final_row;
	const size_t nbr_rows = final_row - initial_row;
	const size_t preamble_length = thread_info->preamble_length;
	mtx_t *mutex = thread_info->mutex;

	/* Strings to hold the rgb values from initial_row to final_row in ppm ascii form */
	char *attractors_string = malloc(nbr_rows * size * NBR_CHARS_PER_LINE * sizeof(char));
	char *convergence_string = malloc(nbr_rows * size * NBR_CHARS_PER_LINE * sizeof(char));

	/* Run newton's method */
	size_t attractor_index;
	size_t nbr_iterations;
	for(size_t row = initial_row; row < final_row; row++) {
		for(size_t col = 0; col < size; col++) {
			/* z is on a grid on [-2, 2)x[-2, 2) \subset C */
			complex double z = ((4.0 * row) / (size)) - 2.0 + ((4.0 * col) / (size) - 2.0) * I;

			/* Perform newton's method with z as initial value */
			newtons_method(
					&nbr_iterations,
					&attractor_index,
					z);

			/* Write down which attractor was approached and nbr of iterations */
			memcpy(&attractors_string[((row - initial_row) * size + col) * NBR_CHARS_PER_LINE],
					attractor_colors[attractor_index],
					NBR_CHARS_PER_LINE * sizeof(char));
			memcpy(&convergence_string[((row - initial_row) * size + col) * NBR_CHARS_PER_LINE],
					convergence_colors[MIN(nbr_iterations, 100) / 11], // TODO: tweak and/or explain
					NBR_CHARS_PER_LINE * sizeof(char));
		}
	}

	//TODO: call write to file as a separate thread
	write_to_file_thread_info_t write_thread_info;
	write_thread_info.initial_row = initial_row;
	write_thread_info.final_row = final_row;
	write_thread_info.preamble_length = preamble_length;
	write_thread_info.attractors_string = attractors_string;
	write_thread_info.convergence_string = convergence_string;
	write_thread_info.mutex = mutex;

	mtx_lock(mutex);
	write_to_file_thread((void*) &write_thread_info);
	mtx_unlock(mutex);


	return 0;
}


/* Writes strings attractors_string_local and convergence_string_local corresponding to lines initial_row to final_row to file */
int write_to_file_thread(void *args) {
	/* Read arguments */
	const write_to_file_thread_info_t *thread_info = (write_to_file_thread_info_t*) args;
	const size_t initial_row = thread_info->initial_row;
	const size_t final_row = thread_info->final_row;
	const size_t nbr_rows = final_row - initial_row;
	const size_t preamble_length = thread_info->preamble_length;
	char *attractors_string = thread_info->attractors_string;
	char *convergence_string = thread_info->convergence_string;
	printf("sizeof(attractors_string) = %zu\n", strlen(attractors_string));


	/* If we're in the last block, terminate string */
	if(final_row == size) {
		attractors_string[nbr_rows * size * NBR_CHARS_PER_LINE - 1] = 0;
		convergence_string[nbr_rows * size * NBR_CHARS_PER_LINE - 1] = 0;
	}

	/* Write to newton_attractors_xd.ppm and newton_convergence_xd.ppm */
	fseek(attractors_file, (preamble_length + initial_row * size * NBR_CHARS_PER_LINE) * sizeof(char), SEEK_SET);
	fseek(convergence_file, (preamble_length + initial_row * size * NBR_CHARS_PER_LINE) * sizeof(char), SEEK_SET);
	fwrite(attractors_string, sizeof(char), nbr_rows * size * NBR_CHARS_PER_LINE, attractors_file);
	fwrite(convergence_string, sizeof(char), nbr_rows * size * NBR_CHARS_PER_LINE, convergence_file);

	free(attractors_string);
	free(convergence_string);


	return 0;
}


/* Given a complex number z, a pointer to nbr_iterations, and a pointer to root_index, performs Newton's iteration method and returns the nbr of iterations required in nbr_iterations and the index in exact_roots to which the result converged, root_index = d means it converged to 0 and root_index = d + 1 means it diverged */
int newtons_method(size_t *nbr_iterations, size_t *attractor_index, double complex z) {
	int has_converged = 0;
	int has_diverged = 0;
	size_t i = 0;

	while(!has_converged && !has_diverged) {
		/* z has converged if it is within 10^-3 of any of the exact roots */
		for(size_t j = 0; j < d + 1; j++) {
			has_converged += (norm_squared(z - exact_roots[j]) < 1e-6); //TODO: condition on f(z) indstead so that we don't have to know the roots beforehand?
		}

		/* z has diverged if its real or imaginary part is larger than 10^10 */
		has_diverged += (logbf(creal(z)) > 33) + (logbf(cimag(z)) > 33); /* 10^10 ~ 2^33 */
		i++;

		/* z = z - f(z) / Df(z) */
		z = (1 / (double) d) * (my_pow(z, 1 - d) + (d - 1) * z); // TODO: FIX THIS
	}
	*nbr_iterations = i;

	/* Check which root z converged to by inflating the euro */
	if (has_converged) {
		for(size_t € = 0; € < d + 1; €++) {
			if(norm_squared((z - exact_roots[€])) < 1e-6) {
				*attractor_index = €;
			}
		}
	} else {
		*attractor_index = d + 1;
	} 


	return 0;
}


complex double my_pow(complex double z, int n) { // TODO: tweak and/or rename
	complex double z2 = z * z;
	complex double z4 = z2 * z2;
	switch(n) {
		case 0:
			return 1;
			break;
		case 1: 
			return z;
			break;
		case 2:
			return z2;
			break;
		case 3:
			return z2 * z;
			break;
		case 4:
			return z4;
			break;
		case 5:
			return z4 * z;
			break;
		case 6:
			return z4 * z2;
			break;
		case 7:
			return z4 * z2 * z;
			break;
		case 8:
			return z4 * z4;
			break;
		case 9:
			return z4 * z4 * z;
			break;
		case 10:
			return z4 * z4 * z2;
			break;
		case -1: 
			return 1 / z;
			break;
		case -2:
			return 1 / z2;
			break;
		case -3:
			return 1 / (z2 * z);
			break;
		case -4:
			return 1 / z4;
			break;
		case -5:
			return 1 / (z4 * z);
			break;
		case -6:
			return 1 / (z4 * z2);
			break;
		case -7:
			return 1 / (z4 * z2 * z);
			break;
		case -8:
			return 1 / (z4 * z4);
			break;
		case -9:
			return 1 / (z4 * z4 * z);
			break;
		case -10:
			return 1 / (z4 * z4 * z2);
			break;
		default:
			fprintf(stderr, "unexpected degree\n");
			exit(1);
	}
}

double norm_squared(complex double z) {
	return creal(z * conj(z));
}

/* returns number of digits of an integer n in base 10 */
inline size_t nbr_digits(size_t n) {
	unsigned int count = 0;
	while (n != 0) {
		n /= 10;
		count++;
	}
	return count;
}
