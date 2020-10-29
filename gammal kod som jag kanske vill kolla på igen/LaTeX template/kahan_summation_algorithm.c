/* Returns sum of array using Kahan summation algorithm. */
double sum(double *array, int array_length) {
	int i; // For summing over
	double term;
	double tmp_sum;
	double sum = 0.0; // Prepare the accumulator.
	double compensator = 0.0; // A running compensation for lost low-order bits.

	for (i = 0; i < array_length; i++) {
		term = array[i] - compensator;
		tmp_sum = sum + term;
		compensator = (tmp_sum - sum) - term;
		sum = tmp_sum;
	}

	return sum;
}
