Without init of as:
We are accessing memory that has not been allocated. 

==153751== Use of uninitialised value of size 8
==153751==    at 0x40116C: main (valgrind_test.c:14)
==153751== 
==153751== 
==153751== Process terminating with default action of signal 11 (SIGSEGV): dumping core
==153751==  Bad permissions for mapped region at address 0x401050
==153751==    at 0x40116C: main (valgrind_test.c:14)

Without free(as):
LEAK SUMMARY:
==154493==    definitely lost: 40 bytes in 1 blocks

With extra free(as):
==155564== Invalid free() / delete / delete[] / realloc()
==155564==    at 0x483B9F5: free (vg_replace_malloc.c:540)
==155564==    by 0x4011ED: main (valgrind_test.c:22)
==155564==  Address 0x4a2f040 is 0 bytes inside a block of size 40 free'd
==155564==    at 0x483B9F5: free (vg_replace_malloc.c:540)
==155564==    by 0x4011E1: main (valgrind_test.c:21)
==155564==  Block was alloc'd at
==155564==    at 0x483A809: malloc (vg_replace_malloc.c:309)
==155564==    by 0x40115E: main (valgrind_test.c:10)

for GDB: We found that the pointer is the null pointer. Program terminated. 





INLINING:
Seems best for inlined manually and same_file. Different file seems slower. 
QUESTION - Will the O2 optimize away some calculations if no "volatile"?

[hpcuser281@gantenbein inlining]$ ./same_file 
Time milliseconds: 0.710726
First element of array: 2.000000
Time milliseconds: 0.709057
First element of array: 2.000000
Time milliseconds: 0.708342
First element of array: 2.000000
[hpcuser281@gantenbein inlining]$ ./different_file 
Time milliseconds: 0.811338
First element of array: 2.000000
Time milliseconds: 0.803947
First element of array: 2.000000
Time milliseconds: 0.775099
First element of array: 2.000000
[hpcuser281@gantenbein inlining]$ ./inlined_manually 
Time milliseconds: 0.706911
First element of array: 2.000000





LOCALITY:
For 1000x1000 matrix with 5000 runs we have:
dgcc -o locality locality.c -O2 -march=native -g 
ms elapsed: 5372.415304
Rowsum 10: 25.038070
ms elapsed: 7671.664715
Colsum 10: -30.959

gcc -o locality locality.c -O2  
ms elapsed: 5380.680084
Rowsum 10: 29.885433
ms elapsed: 7891.522884
Colsum 10: -10.733326

gcc -o locality locality.c -O0  
ms elapsed: 12545.622587
Rowsum 10: 24.065909
ms elapsed: 15500.161648
Colsum 10: 0.173053d222

Added a new colsum2 that does only go through the pointer once, this way
we will never need to jump back and forth in the array. 
now row and col are approx same speed. 

DATA DEP:
Unrolled takes almost halv the time
1024 ms vs 518 ms. Must be because the pipeline is not stalled?...
ms elapsed for row sum: 1016.274929   Rowsum 10: 1008.102539   
ms elapsed for row sum unrolled2: 514.616013   Rowsum unrolled2 10: 1008
ms elapsed for row sum unrolled4: 332.496166   Rowsum unrolled4 10: 1008
ms elapsed for row sum unrolled8: 264.191151   Rowsum unrolled8 10: 1008
We can see that for unrolled 8 we do not get as much of an increase as before
this is because the pipeline is not being stalled to the same extent. 
QUESTION?: Pipeline stall is 4 instructions long? 
QUESTION: What difference does the array do?


INDIRECT ADRESSING:
1000 runs of 1000000 vector.

O0
ms elapsed for linear: 3192.969799            │
x[100]: 0.611194   y[100]: 1222.387577        │
ms elapsed for jumpy: 8323.041916             │
x[100]: 0.611194   y[100]: 2444.775154 

O2
ms elapsed for linear: 1029.443026                                         │
x[100]: 0.247500   y[100]: 494.999236                                  │
ms elapsed for jumpy: 7914.898157                                          │

x[100]: 0.247500   y[100]: 989.998471  
ms elapsed for linear: 1028.158188                     │

O2 native
x[100]: 1.824879   y[100]: 3649.758901                               │
ms elapsed for jumpy: 7300.425053                                                                    │
x[100]: 1.824879   y[100]: 7299.517803  
so more than twice as fast for linear addition. This is because of the
locality of the memory access? Why does O2 make the difference 8-fold?
O2 does not make the jumpy access faster at all? WHY?

AFTER CHANGE TO DIRECT ADRESSING -
O2
ms elapsed for linear: 631.950140
x[100]: 1.720332   y[100]: 3440.663187
ms elapsed for jumpy: 7680.510998
x[100]: 1.720332   y[100]: 6881.326373

O0
ms elapsed for linear: 2190.696955
x[100]: 1.569848   y[100]: 3139.696834
ms elapsed for jumpy: 8497.684002
x[100]: 1.569848   y[100]: 6279.393668



FILES:
for home dir (HDD) O0
ms elapsed for write: 35.207510
ms elapsed for read: 8.149862

O2
ms elapsed for write: 25.736570
ms elapsed for read: 7.87091


FOr bash copy hdd to hdd:
real    0m9.317s
user    0m0.958s                                               
sys     0m7.672s

for ssd to hdd:
real    2m37.600s
user    0m12.260s                                                             
sys     1m43.997

for ssd to ssd.
real    0m8.251s
user    0m0.847s                                                             
sys     0m6.833s 




:wq

