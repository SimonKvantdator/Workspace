import numpy as np
from numpy import sqrt
from numpy import exp
from numpy.linalg import norm

### uppgift 1 ###
m_pi_plus = 139.57 # [MeV]
m_pi_minus = m_pi_plus
p_pi_plus = np.array([3.92, 12.4, 66.2]) # [MeV]
p_pi_minus = np.array([-34.5, -44.6, 1038.0]) # [MeV]

p_gamma_1 = np.array([43.5, 75.1, 64.7]) # [MeV]
p_gamma_2 = np.array([-43.1, 30.5, -38.2]) # [MeV]

m_pi_0 = sqrt((norm(p_gamma_1) + norm(p_gamma_2))**2 - norm(p_gamma_1 + p_gamma_2)**2) # [MeV]
print('m_pi_0 = ', m_pi_0)

p_pi_0 = p_gamma_1 + p_gamma_2 # [MeV]
v_pi_0 = p_pi_0 / sqrt(m_pi_0**2 + norm(p_pi_0)**2) # [1]
print('v_pi_0 = ', v_pi_0)

p_omega = p_pi_plus + p_pi_minus + p_pi_0
m_omega = sqrt((sqrt(m_pi_plus**2 + norm(p_pi_plus)**2) \
        + sqrt(m_pi_minus**2 + norm(p_pi_minus)**2) \
        + sqrt(m_pi_0**2 + norm(p_pi_0)**2))**2 \
        - norm(p_omega)**2)
print('m_omega = ', m_omega)

v_omega = p_omega / sqrt(m_omega**2 + norm(p_omega)**2) # [1]
print('v_omega = ', v_omega)


print('')
### uppgift 2 ###
m = 4 # [GeV]
E = 5 # [GeV]
L = 1e6 # [years c]
tau = 4e5 # [years]
gamma = E / m
v = sqrt(1 - 1 / gamma**2)
print('v = ', v)

T = L / v # [years]
print('T = ', T)

Tp = T / gamma # [years]
print('Tp = ', Tp)
print('livslangd = ', exp(-Tp / tau))


print('')
### uppgift 3 ###
v_s = 0.7
v_p = 0.8
l_vilo = 1 # [m]
gamma = 1 / sqrt(1 - v_s**2)

l = l_vilo / gamma
print('l = ', l)

c = 299792458 # [m / s]
dt = l / (v_p * c - v_s * c)
print('dt = ', dt)


