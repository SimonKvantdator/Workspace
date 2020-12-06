.PHONY : all
all : locality

locality : locality.c
	gcc -o locality_flag=-O0 locality.c -O0 -g;
	gcc -o locality_flag=-O2 locality.c -O2 -g;
	gcc -o locality_flag=-O2\&march=native locality.c -O2 -march=native -g;
