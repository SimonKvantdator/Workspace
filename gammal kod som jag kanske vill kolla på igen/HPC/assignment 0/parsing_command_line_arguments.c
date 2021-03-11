#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>    /* for getopt */

int main(int argc, char *argv[]) {
	int option;
	char *A_string = "not given";
	int A_int;
	char *B_string = "not given";
	int B_int;

	while((option = getopt(argc, argv, "a:b:")) != -1) {
		switch(option) {
			case 'a':
				A_string = optarg;
				A_int = atoi(optarg);
				break;
			case 'b':
				B_string = optarg;
				B_int = atoi(optarg);
				break;
		}
	}

	printf("A is %s and B is %s\n", A_string, B_string);
	return 0;
}
