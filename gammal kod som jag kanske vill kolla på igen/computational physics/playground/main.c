#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define PI 3.141592653589
#define N 2

void main ()
{
	FILE *file;
	int **array = malloc(N * N * sizeof(int));

	array[0][0] = 0;
	array[0][1] = 1;
	array[1][0] = 10;
	array[1][1] = 11;

	if((file=fopen("file.bin", "wb"))==NULL) {
    	printf("Cannot open file :(\n");
	}

	fwrite(array, sizeof(array), 1, file);
	// fread(a, sizeof(int), 1, file);
	fclose(file);
}
