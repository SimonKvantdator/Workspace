/*
 * HPC TMA881 - assignment 3
 * Simon Stefanus Jacobsson 2020-12-16
 *
 * Valgrind output:
 * HEAP SUMMARY:
 *     in use at exit: 0 bytes in 0 blocks
 *   total heap usage: 2,006 allocs, 2,006 frees, 2,009,680 bytes allocated
 * 
 * All heap blocks were freed -- no leaks are possible
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
#define MAX_ARRAY_SIZE 100000
#define DIVERGENCE_TRESHOLD (1e10)
#define NBR_CHARS_PER_LINE 6 /* "R G B\n". Do not confuse LINE as in what's between two newline characters in the ppm file with ROW as in the indes of the real part of z */
#define NBR_ITERATIONS_CAP 100

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

size_t nbr_threads = 1; /* Nbr of threads to run newton iterations with. Initilized now but set later by command line arguments. */
size_t size = 1000;  /* Nbr of rows and columns in the output. Initilized now but set later by command line arguments. */
int d = 7; /* Degree of polynomial. Initilized now but set later by command line arguments. */
complex double exact_roots[MAX_d + 2]; /* {1, e^(2pi/d), e^(4pi/d), ..., e^((d-1)pi/d), 0, inf} */
size_t nbr_rows_computed = 0; /* Total nbr of rows, between all threads, that has been computed */

typedef u_int8_t attractor_t;
typedef u_int8_t convergence_t;
/*typedef size_t attractor_t;*/ // FOR MY OWN MACHINE
/*typedef size_t convergence_t;*/

typedef struct {
	size_t initial_row;
	size_t final_row;
	attractor_t **attractor_indices; /* attractor_indices[row][col] stores the index in exact_roots of the attractor that newton's method converged to with initial value z = 4 * col / size - 2.0 + (4.0 * row / size - 2.0) * I */
	convergence_t **nbrs_iterations; /* nbrs_iterations[row][col] stores the nbr of newton iterations before convergence with initial value z = 4 * col / size - 2.0 + (4.0 * row / size - 2.0) * I */
	int *rows_computed; /* List of rows that has been computed */
	cnd_t *write_condition; /* Signals to the writing thread that a new computation is done */
	mtx_t *mutex;
} computing_thread_info_t;

typedef struct {
	computing_thread_info_t *computing_threads_info;
} writing_thread_info_t;


int computing_thread(void *args);
int writing_thread(void *args);
int newtons_method(attractor_t *attractor_index, convergence_t *nbr_iterations, double complex z);
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
	assert(0 < size && size <= MAX_ARRAY_SIZE);
	assert(0 < nbr_threads && nbr_threads <= size);

	/* Set d and compute the exact roots of z^d - 1 */
	if(argc > 1) {
		d = atoi(argv[argc - 1]); /* d is always the last argument */
	}
	assert(0 < d && d <= MAX_d);
	for(size_t i = 0; i < d; i ++) {
		exact_roots[i] = cexp((complex double) i * 2 * M_PI * I / (complex double) d);
	}
	exact_roots[d] = 0; /* 0 also gets to be a root since it's a stationary point of z -> z - f(z) / f'(c) */


	/* Define threads */
	thrd_t threads[nbr_threads + 1]; /* nbr_threads computing threads + one thread writing to file */
	computing_thread_info_t threads_info[nbr_threads];
	cnd_t write_condition;
	cnd_init(&write_condition);
	mtx_t mutex;
	mtx_init(&mutex, mtx_plain);

	/* Open computing threads. Each computing thread gets a nbr of rows compute the newton iterations for */
	size_t initial_row = 0;
	size_t final_row;
	attractor_t *attractor_indices[size];
	convergence_t *nbrs_iterations[size];
	for (size_t i = 0; i < size; i++) {
		attractor_indices[i] = malloc(size * sizeof(attractor_t));
		nbrs_iterations[i] = malloc(size * sizeof(convergence_t));
	}

	int rows_computed[size];
	for (size_t i = 0; i < size; i++) {
		rows_computed[i] = -1; /* Initialize to -1 */
	}

	for (size_t i = 0; i < nbr_threads; i++) {

		/* Divide up size into nbr_threads blocks as equally as possible */
		size_t final_row = (i < size % nbr_threads) ? \
						   MIN(initial_row + (size / nbr_threads) + 1, size) : \
						   MIN(initial_row + (size / nbr_threads), size);

		threads_info[i].initial_row = initial_row;
		threads_info[i].final_row = final_row;
		threads_info[i].attractor_indices = attractor_indices;
		threads_info[i].nbrs_iterations = nbrs_iterations;
		threads_info[i].rows_computed = rows_computed;
		threads_info[i].write_condition = &write_condition;
		threads_info[i].mutex = &mutex;

		int r;
		r = thrd_create(&threads[i], computing_thread, (void*) &threads_info[i]);
		if(r != thrd_success) fprintf(stderr, "Couldn't create thread\n");

		initial_row = final_row;
	}

	/* Open thread writing to file */
	writing_thread_info_t writing_thread_info; /* The computing threads will communicate with this one by modifying rows_computed */
	writing_thread_info.computing_threads_info = threads_info; /* Pass the computing thread's info to the writing thread */
	int r;
	r = thrd_create(&threads[nbr_threads], writing_thread, (void*) &writing_thread_info);
	if(r != thrd_success) fprintf(stderr, "Couldn't create thread\n");

	/* Close all threads */
	for (size_t i = 0; i < nbr_threads + 1; i++) {
		int r;
		thrd_join(threads[i], NULL);
		if(r != thrd_success) fprintf(stderr, "Couldn't close thread\n");
	}

	for (size_t i = 0; i < size; i++) {
		free(attractor_indices[i]);
		free(nbrs_iterations[i]);
	}
	cnd_destroy(&write_condition);
	mtx_destroy(&mutex);

	return 0;
}


/* Computes rows initial_row to final_row and stores which attractor was approached and how many iterations it took in attractors_string and convergence_string respectively */
int computing_thread(void *args) {
	const computing_thread_info_t *thread_info = (computing_thread_info_t*) args;
	const size_t initial_row = thread_info->initial_row;
	const size_t final_row = thread_info->final_row;
	attractor_t **attractor_indices = thread_info->attractor_indices;
	convergence_t **nbrs_iterations = thread_info->nbrs_iterations;
	int *rows_computed = thread_info->rows_computed;
	cnd_t *write_condition = thread_info->write_condition;
	mtx_t *mutex = thread_info->mutex;

	/* Run newton's method */
	for(size_t row = initial_row; row < final_row; row++) {
		for(size_t col = 0; col < size; col++) {
			/* z is on a grid on [-2, 2)x[-2, 2) \subset C */
			complex double z = ((4.0 * col) / size) - 2.0 + ((4.0 * row) / size - 2.0) * I;

			/* Perform newton's method with z as initial value and store results in attractor_indices[row][col] and nbrs_iterations[row][col] */
			newtons_method(
					&attractor_indices[row][col],
					&nbrs_iterations[row][col],
					z);
		}

		/* Tell the writing thread that this row has been computed */
		mtx_lock(mutex);
		rows_computed[nbr_rows_computed] = row;
		nbr_rows_computed++;
		mtx_unlock(mutex);
		cnd_signal(write_condition);
	}

	return 0;
}


/* Writes strings attractors_string_local and convergence_string_local corresponding to lines initial_row to final_row to file */
int writing_thread(void *args) {
	const writing_thread_info_t *thread_info = (writing_thread_info_t*) args;
	/* Every thread should have the same of these, so we just take them from the first */
	attractor_t **attractor_indices = thread_info->computing_threads_info[0].attractor_indices;
	convergence_t **nbrs_iterations = thread_info->computing_threads_info[0].nbrs_iterations;
	int *rows_computed = thread_info->computing_threads_info[0].rows_computed;
	cnd_t *write_condition = thread_info->computing_threads_info[0].write_condition;

	char attractors_string[size * NBR_CHARS_PER_LINE];
	char convergence_string[size * NBR_CHARS_PER_LINE];

	/* Open ppm files for writing results to */
	FILE *attractors_file; /* File to write which roots we converge to */
	FILE *convergence_file; /* File to write how many iterations before we converge to */
	char attractors_filename[26];
	char convergence_filename[26];
	sprintf(attractors_filename, "newton_attractors_x%i.ppm", d); //NOLINT
	sprintf(convergence_filename, "newton_convergence_x%i.ppm", d); //NOLINT
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
	sprintf(preamble, "P3\n%zu %zu\n%i\n", size, size, 9); //NOLINT
	size_t preamble_length = strlen(preamble); /* Nbr of chars of the preamble to the ppm file */
	fwrite(preamble, sizeof(char), preamble_length, attractors_file);
	fwrite(preamble, sizeof(char), preamble_length, convergence_file);


	/* Each attractor gets represented by a color and each bin of nbr of iterations gets represented by a shade */
	FILL_ATTRACTOR_COLORS(attractor_colors);
	FILL_CONVERGENCE_COLORS(convergence_colors);

	/* Fill body of ppm file */
	size_t i = 0;
	while (i < size) {
		/* If another row have been calculated, write it to file. Otherwise, sleep. */
		size_t row = rows_computed[i];
		if(row != -1) {
			/* Write the results of this row to a string */
			for(size_t col = 0; col < size; col++) {
				memcpy(&attractors_string[col * NBR_CHARS_PER_LINE], //NOLINT
						attractor_colors[attractor_indices[row][col]],
						NBR_CHARS_PER_LINE * sizeof(char));
				memcpy(&convergence_string[col * NBR_CHARS_PER_LINE], //NOLINT
						convergence_colors[MIN(nbrs_iterations[row][col], NBR_ITERATIONS_CAP) / (NBR_ITERATIONS_CAP / 10 + 1)], /* MIN(nbrs_iterations[row][col], NBR_ITERATIONS_CAP) / (NBR_ITERATIONS_CAP / 10 + 1) puts the nbr of iterations on a scale from 0 to 9 */
						NBR_CHARS_PER_LINE * sizeof(char));
			}
			/* Write that string to file */
			fseek(attractors_file, (preamble_length + row * size * NBR_CHARS_PER_LINE) * sizeof(char), SEEK_SET);
			fseek(convergence_file, (preamble_length + row * size * NBR_CHARS_PER_LINE) * sizeof(char), SEEK_SET);
			fwrite(attractors_string, sizeof(char), size * NBR_CHARS_PER_LINE, attractors_file);
			fwrite(convergence_string, sizeof(char), size * NBR_CHARS_PER_LINE, convergence_file);

			i++;
		} else {
			/*cnd_wait(write_condition, mutex);	 //THIS IS CONSISTENTLY A LITTLE BIT SLOWER*/
			usleep(1);
		}
	}

	fclose(attractors_file);
	fclose(convergence_file);

	return 0;
}


/* Given a complex number z, a pointer to nbr_iterations, and a pointer to root_index, performs Newton's iteration method and returns the nbr of iterations required in nbr_iterations and the index in exact_roots to which the result converged, root_index = d means it converged to 0 and root_index = d + 1 means it diverged */
int newtons_method(attractor_t *attractor_index, convergence_t *nbr_iterations, double complex z) {
	int has_converged = 0;
	int has_diverged = 0;
	size_t i = 0;
	size_t j = 0;

	while(!has_converged && !has_diverged) {
		/* z = z - f(z) / Df(z) */
		z = (1 / (double) d) * (my_pow(z, 1 - d) + (d - 1) * z);

		/* z has converged if it is within 10^-3 of any of the exact roots */
		for(j = 0; j < d + 1; j++) {
			if(norm_squared(z - exact_roots[j]) < 1e-6) {
				has_converged = 1;
				break;
			}
		}

		/* z has diverged if its real or imaginary part is larger than 10^10 */
		if ((fabs(creal(z)) > DIVERGENCE_TRESHOLD) || (fabs(cimag(z)) > DIVERGENCE_TRESHOLD)) {
			has_diverged = 1;
		}

		i++;
	}
	*nbr_iterations = i;
	if(has_converged) {
		*attractor_index = j;
	} else {
		*attractor_index = d + 1;
	}

	return 0;
}

complex double my_pow(complex double z, int n) {
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
			return 1.0 / 0.0;
	}
}

inline double norm_squared(complex double z) {
	return creal(z) * creal(z) + cimag(z) * cimag(z);
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
