#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void stack_allocation() {

	int array_size = pow(10, 6); // Segmentation fault at array size 10^7
	int array[array_size];

	for(int i = 0; i < array_size; ++i) {
		array[i] = 0;
	}

	printf("%d\n", array[0]);
}


void heap_allocation() {

	int array_size = pow(10, 7);
	int *array = (int*) malloc(sizeof(int) * array_size);
	for (int i = 0; i < array_size; ++i) {
		array[i] = 0;
	}

	printf("%d\n", array[0]);

	free(array);
}


int main() {
	stack_allocation();
	/*
	 * Segmentation fault means that I tried to access memory that I did
	 * not have access to. In this case it's because that memory does
	 * not exist.
	 */

	heap_allocation();
	
	return 0;
}
