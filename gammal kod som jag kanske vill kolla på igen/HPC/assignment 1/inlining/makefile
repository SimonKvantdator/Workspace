COMPILER = gcc
FLAGS = -O2 -march=native -g

.PHONY : all
all : \
	same_file \
	different_file \
	inline_manually
	
same_file : \
	same_file.c \
	constants.h \
	$(COMPILER) $(FLAGS) -o $@ $< 

different_file : \
	different_file .c \
	constants.h \
	$(COMPILER) $(FLAGS) -o $@ different_file_mul.c $< 

inline_manually : \
	inline_manually.c \
	constants.h \
	$(COMPILER) $(FLAGS) -o $@ $< 
