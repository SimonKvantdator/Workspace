#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <threads.h>

#define NBR_THREADS 4
#define ARRAY_SIZE 4
typedef struct {
	int *array;
	size_t array_size;
	size_t array_index;
} thread_info_t;

int main_thread(void *args) {
	const thread_info_t *thread_info = (thread_info_t*) args;
	int *array = thread_info->array;
	int array_size = thread_info->array_size;
	int array_index = thread_info->array_index;
	assert(array_index < array_size);
	
	array[array_index]++;

	return 0;
}

int main() {
	/* Initialize array */
	int array[ARRAY_SIZE] = {0}; 

	/* Initialize threads */
	thrd_t threads[NBR_THREADS];
	thread_info_t threads_info[NBR_THREADS];
	for(size_t i = 0; i < NBR_THREADS; i++) {
		threads_info[i].array = array;
		threads_info[i].array_size = ARRAY_SIZE;
		threads_info[i].array_index = i;

		int r = thrd_create(threads + i, main_thread, (void *) (threads_info + i));
		if(r != thrd_success) {
			fprintf(stderr, "failed to create thread\n");
			exit(1);
		}

		/* Increment each element in array */
		int R;
		thrd_join(threads[i], &R);
	}

	printf("array = [%i, %i, %i, %i]\n", array[0], array[1], array[2], array[3]);

	return 0;


}
