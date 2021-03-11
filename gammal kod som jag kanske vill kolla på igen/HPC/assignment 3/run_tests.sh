#!/bin/bash

#make
#./newton -t1 -l100 1
#./newton -t1 -l100 2
#./newton -t1 -l100 3
#./newton -t1 -l100 4
#convert newton_attractors_x1.ppm pictures/newton_attractors_x1.png
#convert newton_attractors_x2.ppm pictures/newton_attractors_x2.png
#convert newton_attractors_x3.ppm pictures/newton_attractors_x3.png
#convert newton_attractors_x4.ppm pictures/newton_attractors_x4.png
#convert newton_convergence_x1.ppm pictures/newton_convergence_x1.png
#convert newton_convergence_x2.ppm pictures/newton_convergence_x2.png
#convert newton_convergence_x3.ppm pictures/newton_convergence_x3.png
#convert newton_convergence_x4.ppm pictures/newton_convergence_x4.png

rm -vR extracted/ pictures/
tar czf newton.tar.gz makefile newton.c
/home/hpc2020/newton_iteration/check_submission.py /home/hpcuser326/assignment\ 3/newton.tar.gz

scp pictures/* simon@46.239.103.185:~
