#Använd semiempiriska massformeln för att bestämma vilken
# A=64-isobar med jämnt antal protoner som är mest stabil
import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt

def is_odd(N):
	if (N % 2) == 0:
		return 

a_v = 15.56 # [MeV / c^2]
a_s = 17.23 # [MeV / c^2]
a_c = 0.697 # [MeV / c^2]
a_a = 93.14 # [MeV / c^2]
a_p = 12. # [MeV / c^2]

m_p = 938.272 # [MeV / c^2]
m_n = 939.565 # [MeV / c^2]
m_e = 0.511 # [MeV / c^2]

def M(Z, A):
	if is_odd(A):
		delta = 0.
	if is_odd(A - Z) and is_odd(Z):
		delta = 1.
	else:
		delta = -1.

	return Z * (m_p + m_e) +\
		(A - Z) * m_n +\
		-a_v * A +\
		a_s * A**(2. / 3.) +\
		a_c * Z**2 / A**(1. / 3.) +\
		a_a * (Z - A / 2.)**2 / A +\
		delta * a_p * A**(-1. / 2.)

Z_list = [i for i in range(0, 100)]
M_list = [M(Z, 64) for Z in Z_list]

fig, ax = plt.subplots()
ax.plot(Z_list, M_list, 'o')
ax.set_xlabel('Z')
ax.set_ylabel('M(Z, 64) [MeV / c^2]')

print(f'Z = {np.array(Z_list)[M_list == np.min(M_list)][0]} minimizes M(Z, 64)')
plt.show()



