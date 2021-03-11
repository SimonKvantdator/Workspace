.PHONY : all
all : valgrind gdb

valgrind : valgrind.c
	gcc -O0 -o valgrind valgrind.c -g

gdb : gdb.c
	gcc -O0 -o gdb gdb.c -g
