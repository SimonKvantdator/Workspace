#include <stdio.h>

int write_matrix_to_file() {
	int array_size = 10;
	int array[array_size][array_size]; // 2D square array of size array_size
	FILE *array_file;

	// put i*j in the i-th row and j-th column of array
	for(int i = 0; i < array_size; i++) {
		for(int j = 0; j < array_size; j++) {
			array[i][j] = i * j;
		}
	}

	// write array to arrat.dat
	array_file = fopen("array.dat", "w");
	if(array_file == NULL) {
		printf("error opening file\n");
		return -1;
	}
	fwrite(array, sizeof(int), array_size * array_size, array_file);
	fclose(array_file);

	return 0;
}


int read_matrix_from_file() {
	int array_size = 10;
	int array[array_size][array_size]; // 2D square array of size array_size
	FILE *array_file;

	// read array from array.dat
	array_file = fopen("array.dat", "r");
	if(array_file == NULL) {
		printf("error opening file\n");
		return -1;
	}
	fread(array, sizeof(int), array_size * array_size, array_file);
	fclose(array_file);

	// check that i*j is in the i-th row and j-th column of array
	printf("truth values of array[i][j] == i * j:\n");
	for(int i = 0; i < array_size; i++) {
		for(int j = 0; j < array_size; j++) {
			printf("%d", array[i][j] == i * j);
		}
		printf("\n");
	}

	return 0;
}


int main() {

	/*return write_matrix_to_file();*/
	// if we allocate non-contiguous, we will have to write each subarray to file separately in a for-loop
	
	return read_matrix_from_file();
}
