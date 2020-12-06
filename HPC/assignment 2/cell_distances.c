/*
 * HPC TMA881 - assignment 2
 * Simon Stefanus Jacobsson 2020-12-06
 *
 * This program will never consume more than 1 GiB of memory since in the worst case scenario, nbr_cells = 2^32. Then nbr_cells_per_block = 2^16, and so
 *   	sizeof(cells1) + sizeof(cells2) + sizeof(cells_str) + nbr_threads * sizeof(frequencies) + sizeof(output)
 *   	<= nbr_cells_per_block * 3 * sizeof(fix) + nbr_cells_per_block * 3 *sizeof(fix) + nbr_cells_per_block * NBR_CHARS_PER_LINE * sizeof(char) + 20 * 3465 * sizeof(unsigned int) + (7 + nbr_digits(file_length)) * NBR_BINS_PER_BLOCK * sizeof(char)
 *   	= 20 * 2^16 (3 * 16 + 3 * 16 + 24 * 8) + 20 * 3465 * 16 + (7 + 5) * 3465 b
 *   	= 2.4 MiB
 *   	So even if I run on 20 cores, I will never use more than 1 GiB.
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <unistd.h>  /* For getopt */
#include <omp.h>

#include <math.h> /* For sqrtf */

#define NBR_CHARS_PER_LINE 24  /* Each line in cells has 24 characters */
#define BIN_SIZE 0.01
#define BIN2FIX 10 /* = BIN_SIZE * FIX2INT */
#define NBR_BINS 3465 /* There are 3465 bins of size 0.01 between 0.00 and |(10.0, 10.0, 10.0) - (-10.0, -10.0, -10.0)| = 34.64 */

typedef short int fix; /* Fixed point number type, representing 12.345 as a short int 12345 */
typedef unsigned short int unsigned_fix; /* Unsigned fixed point number type, representing 12.345 as an unsigned short int 12345 */

static inline void parse_3vector(fix r[3], char *string);
static inline unsigned_fix distance(fix r1[3], fix r2[3]);
static inline unsigned int isqrt(unsigned int n);
static inline size_t nbr_digits(size_t n);


int main(int argc, char *argv[]) {
	unsigned int frequencies[NBR_BINS] = {0}; /* Frequency of distance 0.0 is stored in frequencies[0], 0.01 in frequencies[1], and so on */

	/* Set nbr of threads */
	int option;
	while((option = getopt(argc, argv, "t:")) != -1) {
		switch(option) {
			case 't':
				omp_set_num_threads(atoi(optarg));
				break;
		}
	}


	FILE *cells_file = fopen("cells", "r");
	if (cells_file == NULL) {
		printf("error opening file\n");
		return -1;
	}

	/* Count nbr of cells by counting nbr of lines in file cells */
	fseek(cells_file, 0, SEEK_END);
	size_t file_length = ftell(cells_file); /* [B] */
	size_t nbr_cells = file_length / NBR_CHARS_PER_LINE; /* Each character is 1 B */
	fseek(cells_file, 0, SEEK_SET);

	/* Nbr of blocks to divide cells into. Choose nbr_blocks to be the greatest number less than sqrt(nbr_cells) that divides nbr_cells */
	size_t nbr_blocks = isqrt(nbr_cells);
	while (nbr_cells % nbr_blocks) { 
		nbr_blocks--;
	}
	size_t nbr_cells_per_block = nbr_cells / nbr_blocks;

	/* Loop over when the blocks are different */
	for (size_t block_index = 0; block_index < ((nbr_blocks - 1) * nbr_blocks) / 2; block_index++) {

		/* To avoid counting blocks twice, identify
		 * block_index	|	block_index1	block_index2
		 * 0			|	1				0
		 * 1			|	2				0
		 * 2			|	2				1
		 * 3			|	3				0
		 * 4			|	3				1
		 * 5			|	3				2
		 * etc */
		size_t block1_index = floor(0.5 * (isqrt(8 * block_index + 1) - 1.0)) + 1;
		size_t block2_index = block_index - ((block1_index - 1) * block1_index) / 2;

		/* Read cells from file */
		fix cells1[nbr_cells_per_block][3];
		char cells_str[nbr_cells_per_block * NBR_CHARS_PER_LINE];
		fseek(cells_file, block1_index * nbr_cells_per_block * NBR_CHARS_PER_LINE, SEEK_SET); /* Each character is 1 byte */
		fread(cells_str, NBR_CHARS_PER_LINE * sizeof(char), nbr_cells_per_block, cells_file);
		for(size_t cell_index = 0; cell_index < nbr_cells_per_block; cell_index++) {
			parse_3vector(cells1[cell_index], &cells_str[cell_index * NBR_CHARS_PER_LINE]);
		}

		fix cells2[nbr_cells_per_block][3];
		fseek(cells_file, block2_index * nbr_cells_per_block * NBR_CHARS_PER_LINE, SEEK_SET);
		fread(cells_str, NBR_CHARS_PER_LINE * sizeof(char), nbr_cells_per_block, cells_file);
		for(size_t cell_index = 0; cell_index < nbr_cells_per_block; cell_index++) {
			parse_3vector(cells2[cell_index], &cells_str[cell_index * NBR_CHARS_PER_LINE]);
		}

		/* Compute the distances between each pair of cells */
		/* When comparing different blocks we want to include all (cell1_index, cell2_index) = (0, 0), (0, 1), (1, 0), (1, 1), etc */
		#pragma omp parallel for reduction(+:frequencies[:3465])
		for(size_t cell1_index = 0; cell1_index < nbr_cells_per_block; cell1_index++) {
			for(size_t cell2_index = 0; cell2_index < nbr_cells_per_block; cell2_index+=1) {

				/* Which size-0.01-bin is the distance in? */
				size_t bin = distance(cells1[cell1_index], cells2[cell2_index])   / BIN2FIX;
				frequencies[bin]++;
			}
		}
	}

	/* Loop over when blocks are the same */
	for (size_t block_index = 0; block_index < nbr_blocks; block_index++) {

		/* Read cells from file */
		fix cells1[nbr_cells_per_block][3];
		char cells_str[nbr_cells_per_block][NBR_CHARS_PER_LINE];
		fseek(cells_file, block_index * nbr_cells_per_block * NBR_CHARS_PER_LINE, SEEK_SET); /* Each character is 1 byte */
		fread(cells_str, NBR_CHARS_PER_LINE * sizeof(char), nbr_cells_per_block, cells_file);
		for(size_t cell_index = 0; cell_index < nbr_cells_per_block; cell_index++) {
			parse_3vector(cells1[cell_index], cells_str[cell_index]);
		}

		fix cells2[nbr_cells_per_block][3];
		memcpy(cells2, cells1, nbr_cells_per_block * 3 * sizeof(fix));


		/* Compute the distances between each pair of cells */
		/* When comparing a block to itself we only want to include (cell1_index, cell2_index) = (0, 0), (1, 0), (2, 0), (2, 1), (3, 0), etc */
		size_t nbr_distances = ((nbr_cells_per_block - 1) * nbr_cells_per_block) / 2; /* When comparing different blocks we want to exclude the case cell_index1 = cell_index2 */
		#pragma omp parallel for reduction(+:frequencies[:3465])
		for(size_t cell_index = 0; cell_index < nbr_distances; cell_index++) {
			size_t cell1_index = floor(0.5 * (isqrt(8 * cell_index + 1) - 1.0)) + 1;
			size_t cell2_index = cell_index - (cell1_index - 1) * cell1_index / 2;

			size_t bin = distance(cells1[cell1_index], cells2[cell2_index]) / BIN2FIX;
			frequencies[bin]++;
		}
	}

	fclose(cells_file);

	/* Print frequencies */
	char *output = malloc((7 + nbr_digits(file_length)) * NBR_BINS * sizeof(char)); /* Each bin is 7 + nbr_digits(frequencies[bin]) chars */
	output[0] = 0;
	for(size_t bin = 0; bin < NBR_BINS; bin++) {
		if(frequencies[bin]) {
			char tmp_string[20];
			sprintf(tmp_string, "%05.2f %zu\n", (float) bin * BIN_SIZE, frequencies[bin]);
			strcat(output, tmp_string);
		}
	}
	printf(output);
	free(output);

	return 0;
}


/* Given a string of the form "+01.234 -02.345 +03.456", parses it as a 3-vector and returns it in r */
inline void parse_3vector(fix r[3], char *string) {
	short int sign = (string[0] == '-') ? -1 : 1;
	r[0] = sign * (
			(string[1] - '0') * 10000 +
			(string[2] - '0') * 1000 +
			(string[4] - '0') * 100 +
			(string[5] - '0') * 10 +
			(string[6] - '0') * 1);

	sign = (string[8] == '-') ? -1 : 1;
	r[1] = sign * (
			(string[9]  - '0') * 10000 +
			(string[10] - '0') * 1000 +
			(string[12] - '0') * 100 +
			(string[13] - '0') * 10 +
			(string[14] - '0') * 1);

	sign = (string[16] == '-') ? -1 : 1;
	r[2] = sign * (
			(string[17] - '0') * 10000 +
			(string[18] - '0') * 1000 +
			(string[20] - '0') * 100 +
			(string[21] - '0') * 10 +
			(string[22] - '0') * 1);
}

/* Returns distance between two 3-vectors */
inline unsigned_fix distance(fix r1[3], fix r2[3]) {
	int x1 = r1[0];
	int x2 = r2[0];
	int y1 = r1[1];
	int y2 = r2[1];
	int z1 = r1[2];
	int z2 = r2[2];

	int dx = x1 - x2;
	int dy = y1 - y2;
	int dz = z1 - z2;

	return (unsigned_fix) sqrtf(dx * dx + dy * dy + dz * dz);
}

/* Integer sqrt method */
inline unsigned int isqrt(unsigned int n) {
	return (unsigned int) sqrt((double) n); /* Double type actually large enough for this algorithm to be exact for all 32 bit integers */
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
