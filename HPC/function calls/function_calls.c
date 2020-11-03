#include <stdio.h>

void printf_from_2nd_argument(int, char*[]);


int main(int argc, char *argv[]) {

	printf_from_2nd_argument(argc, argv);
	return 0;
}

void printf_from_2nd_argument(int argc, char *argv[]) {

	for (int i = 0; i < argc; ++i) {
		printf("%s\n", argv[i]);	
	}
}
