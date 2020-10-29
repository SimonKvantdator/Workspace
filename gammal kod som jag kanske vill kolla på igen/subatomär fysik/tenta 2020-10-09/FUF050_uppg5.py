# Vi tar scriptet från uppg 3 till att ta hänsyn till
# att vi kan ha olika många elektronemånga elektroner.
# se kommentarerna nedan.
import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt

def is_odd(N):
	if (N % 2) == 0:
		return 1.0

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

# ~80 protoner => ~120 neutroner (avläses från en nuklidkarta)
# så vi testar lite olika antal neutroner däromkring
for nbr_neutrons in range(115, 125):
    Z1 = 82
    Z2 = 84
    A1 = Z1 + nbr_neutrons
    A2 = Z2 + nbr_neutrons
    
    deltaE_82 = M(Z1, A1) - M(Z1-2, A1-4)
    deltaE_84 = M(Z2, A2) - M(Z2-2, A2-4)
    print('deltaE when Z=82 is ', deltaE_82, 'MeV')
    print('deltaE when Z=84 is ', deltaE_84, 'MeV')
    print('their difference is ', deltaE_84 - deltaE_82, 'MeV')
    print()


