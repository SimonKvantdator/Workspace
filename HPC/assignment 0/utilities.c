#include "utilities.h"

/* Integer power operator */
integer ipow(integer a, int n) {
	int i;
	double tmp = 1.0;

	for (i = 0; i < n; i++) {
		tmp *= a;
	}

	return tmp;
}
