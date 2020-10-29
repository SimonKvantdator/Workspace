import matplotlib as mpl
import matplotlib.pyplot as plt
import numpy as np
from scipy.integrate import ode # solves y'(t) = f(y, t)
c = 1
m_p = 938.2720813 # [MeV / c^2]
Z = 1

T0 = 60.0 # [MeV]
x0 = 0 # [m]
y0 = [T0, x0]
t0 = 0.0 # [m / c]

def dTdx(v): # approximate Bethe-Bloch formula
	beta = v / c
	return -1.7 * Z**2 / beta**2 # [MeV / m]

def f(t, y):
	T = y[0]
	x = y[1]
	v = np.sqrt(2.0 * T / m_p)
	return [dTdx(v) * v, v] # [MeV c / m], [c]


r = ode(f).set_integrator('zvode', method='bdf')
r.set_initial_value(y0, t0)
dt = 1e-4 # [m / c]
t1 = 1e4 * dt # [m / c]

T_list = np.zeros(int(t1 / dt) + 1) * np.nan
x_list = np.zeros(int(t1 / dt) + 1) * np.nan
t_list = np.zeros(int(t1 / dt) + 1) * np.nan
i = 0
while r.successful() and r.t < t1:
	T_list[i] = np.real(r.integrate(r.t + dt)[0])
	x_list[i] = np.real(r.integrate(r.t + dt)[1])
	t_list[i] = r.t
	i = i + 1

fig, ax = plt.subplots()
ax.plot(x_list, T_list)
ax.set_xlabel('x')
ax.set_ylabel('T')
plt.show()

print(f'x_max = {max(x_list):.4f} [m]')