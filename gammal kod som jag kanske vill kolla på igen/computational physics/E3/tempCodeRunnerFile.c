	MC_mean = mean(MC_list, MC_LENGTH);
	for (i = 1; i < MC_LENGTH; i++) {
		printf("MC_list[i] = %f\n", MC_list[i]);
		MC_list[i] -= MC_mean;
	}
