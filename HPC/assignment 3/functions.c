#include "functions.h"
#include <complex.h>
#include <stdio.h>


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
			return 1.0 / 0.0;
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
