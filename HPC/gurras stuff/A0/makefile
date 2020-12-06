# Usage:
# make
# make clean
# remove all binaries and objects

.PHONY : all clean

#LINKERFLAG = -lm

SRCS := $(wildcard *.c)
BINS := $(SRCS:%.c=%)

all: ${BINS}

#%: %.o
#	@echo "Checking..."
#${CC} ${LINKERFLAG} $< -o $@

% : %.c
	@echo "Creating object..."
	gcc -o $@ $<

clean:
	@echo "Cleaning up..."
	rm -rvf *.o ${BINS}
	
