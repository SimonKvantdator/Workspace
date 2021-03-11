.PHONY : all
all : indirect_adressing

indirect_adressing : indirect_adressing.c
	gcc -o indirect_adressing_flag=-O0 indirect_adressing.c -O0 -g;
	gcc -o indirect_adressing_flag=-O2 indirect_adressing.c -O2 -g;
	gcc -o indirect_adressing_flag=-O2\&march=native indirect_adressing.c -O2 -march=native -g;
