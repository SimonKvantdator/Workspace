#include <assert.h>

void multiply_by_2(int x) {
	x = 2 * x;
}

int main() {
	int x = 3;

	multiply_by_2(x);
	assert(x == 6);
}


