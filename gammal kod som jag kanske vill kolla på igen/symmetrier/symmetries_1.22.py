import numpy as np
from math import factorial

### take some matrix in sl(C²) ###
dim = 2
a = -1.
b = -2.
c = -3.
A = np.array([[a,  b],
              [c, -a]])
det_A = -(a**2 + b * c)
print(f'det[A] = {det_A} \n')

### define matrix exponantiation ###
def matrix_exp(M):
    tmp_sum = np.zeros(M.shape)
    M_power_i = np.eye(dim)

    for i in range(20):
        tmp_sum = tmp_sum + M_power_i / factorial(i)
        M_power_i = M_power_i @ M
    
    return tmp_sum #assume convergence etc

### print LHS & RHS from notes ###
if (det_A > 0.):
    u = np.sqrt(det_A)
    print('exp[A] = \n', matrix_exp(A), '\n')
    print('cos[det[A]] I + (1/det[A]) sin[det[A]] A = \n', np.cos(u) * np.eye(dim) + np.sin(u) * A / u, '\n')
else:
    u = np.sqrt(-det_A)
    print('exp[A] = \n', matrix_exp(A), '\n')
    print('cos[det[A]] I + (1/det[A]) sin[det[A]] A = \n', np.cosh(u) * np.eye(dim) + np.sinh(u) * A / u, '\n')

