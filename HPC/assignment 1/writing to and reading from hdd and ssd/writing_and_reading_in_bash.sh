#!/bin/bash

time \
for i in {1 ... 10}; do
	cp --recursive include/* include_copy/
done

rm --recursive include_copy/* # clean include_copy
exit 0
# On HDD:
# real    0m3,952s
# user    0m0,280s
# sys     0m3,284s
#
# On SSD:
# real    0m4,331s
# user    0m0,299s
# sys     0m3,466s
