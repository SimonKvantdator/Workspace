#include <stdio.h>
#include <stdlib.h>

void not_avoiding_memory_fragmentation() {

	int array_size = 10; // nbr of rows in array
	int **array = (int**) malloc(sizeof(int*) * array_size); // initiate 2D array, or array of pointers. But these pointers may point to wherever
	for (int i = 0; i < array_size; ++i) {
		array[i] = (int*) malloc(sizeof(int) * array_size); // initiate each row
	}

	for (int i = 0; i < array_size; ++i) {
		for (int j = 0; j < array_size; ++j) {
			array[i][j] = 0;
		}
	}

	printf("%d\n", array[0][0]);

	for (int i = 0; i < array_size; ++i) {
		free(array[i]);
	}
	free(array);
}


void avoiding_memory_fragmentation() {

	int array_size = 10;
	int *array_entries = (int*) malloc(sizeof(int) * array_size * array_size); // Declare single array with the intended size of our 2D array
	int **array = (int**) malloc(sizeof(int*) * array_size);
	for (int i = 0, j = 0; i < array_size; ++i, j+=array_size) {
		array[i] = array_entries + j; // point all the 1D arrays in our 2D array to contigous places in array_entries, which we have constructed to be just large enough to fit all of the 1D arrays
	}

	for (int i = 0; i < array_size; ++i) {
		for (int j = 0; j < array_size; ++j) {
			array[i][j] = 0;
		}
	}

	printf("%d\n", array[0][0]);

	free(array);
	free(array_entries);
	// we don't need the freeing for-loop as in not_avoiding_memory_fragmentation() since we already freed the memory by freeing array_entries
}


int main() {

	not_avoiding_memory_fragmentation();
	avoiding_memory_fragmentation();

	return 0;
}
