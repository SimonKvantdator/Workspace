.PHONY : all
all : data_dependency

data_dependency : data_dependency.c
	gcc -o data_dependency_flag=-O0 data_dependency.c -O0 -g;
	gcc -o data_dependency_flag=-O2 data_dependency.c -O2 -g;
	gcc -o data_dependency_flag=-O2\&march=native data_dependency.c -O2 -march=native -g;
