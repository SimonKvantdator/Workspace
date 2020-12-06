#!/bin/bash
rm -vR distances/ extracted/
tar czf cell_distances.tar.gz makefile cell_distances.c
/home/hpc2020/cell_distances/check_submission.py /home/hpcuser326/assignment\ 2/cell_distances.tar.gz
# ./check_submission.py cell_distances.tar.gz

