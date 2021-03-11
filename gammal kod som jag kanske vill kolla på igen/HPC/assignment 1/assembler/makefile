.PHONY : all
all : naive_benchmarking

naive_benchmarking : naive_benchmarking.c
	for OPTIMIZATION_FLAG in -O0 -O1 -O2 -O3 -Os -Og; do \
		gcc $$OPTIMIZATION_FLAG -o naive_benchmarking_flag=$$OPTIMIZATION_FLAG naive_benchmarking.c -g; \
	done
