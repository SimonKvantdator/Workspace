# Usage:
# make
# make clean
# remove all binaries and objects

.PHONY : clean

#LINKERFLAG = -lm

SRCS := $(wildcard *.c)
BINS := $(SRCS:%.c=%)

#all: ${BINS}

#%: %.o
#	@echo "Checking..."
#${CC} ${LINKERFLAG} $< -o $@

indadd : indadd.c
	@echo "Creaing same_file..."
	gcc -o $@ $^ -g -O2

clean:
	@echo "Cleaning up..."
	rm -rvf *.o ${BINS}
	
