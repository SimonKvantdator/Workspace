import numpy as np
import matplotlib as mpl
from matplotlib import pyplot as plt

# ### for plotting nice plots ###
width = 6
height = width / 1.5
fsize = 16
font = {'size': fsize}
mpl.rc('font', **font)
mpl.rc('xtick', labelsize=fsize)
mpl.rc('ytick', labelsize=fsize)
mpl.rc('text', usetex=False)

# ### nature constants ###
hydrogen_mass = 104.5 # [eV fs^2 / A^2]

# ### parameters ###
a = 0.3 # [eV]
b = 0.4 # [A]
c_list = [0.05, 0.1] # [A]
d = 0.7 # [A]

# ### returns potential in 2x2 matrix ###
def V(x, a, b, c, d):

	V11 = a * (2 * np.heaviside(x, 1.0) - np.sign(x) * np.exp(-np.sign(x) * x / b))

	V12 = c * np.exp(-(x / d)**2)
	V21 = V12

	V22 = 2 * a - V11

	return np.array([[V11, V12], [V21, V22]])

### make lists of V11 & V22 from undiagonalized potential and plot ###
c = 0.0 # V11 and V22 does not depend on c so just initiate it
fig, ax = plt.subplots(nrows=1, ncols=1, figsize=(8,4))
x_list = np.linspace(-2.0, 2.0, num=200)
V11_diabatic_list = V(x_list, a, b, c, d)[0, 0]
V12_diabatic_list = V(x_list, a, b, c, d)[0, 1]
V21_diabatic_list = V(x_list, a, b, c, d)[1, 0]
V22_diabatic_list = V(x_list, a, b, c, d)[1, 1]

### plot V11 & V22 ###
ax.plot(x_list, V11_diabatic_list, color='C0', linewidth=2.0, label=r'$V_{11}$')
ax.plot(x_list, V22_diabatic_list, color='C1', linewidth=2.0, label=r'$V_{22}$')
ax.set_xlabel(r'$x$ [A]')
ax.set_ylabel(r'[eV]')

### legend & save file ###
plt.legend()
plt.tight_layout()
filename = f'figures/diabatic_task4.pdf'
plt.savefig(filename)
filename = f'figures/diabatic_task4.png'
plt.savefig(filename)
plt.show()



### make lists of V11 & V22 from diagonalized potential and plot ###
fig, ax = plt.subplots(nrows=1, ncols=1, figsize=(8,4))
x_list = np.linspace(-2.0, 2.0, num=200)
linestyles=['-', '--']
for (c, linestyle) in zip(c_list, linestyles):
	V11_diabatic_list = V(x_list, a, b, c, d)[0, 0]
	V12_diabatic_list = V(x_list, a, b, c, d)[0, 1]
	V21_diabatic_list = V(x_list, a, b, c, d)[1, 0]
	V22_diabatic_list = V(x_list, a, b, c, d)[1, 1]

	# 1/2 (V11 + V22 - Sqrt[V11^2 + 4 V12 V21 - 2 V11 V22 + V22^2])
	V11_adiabatic_list = (1/2) * (V11_diabatic_list + V22_diabatic_list - np.sqrt(V11_diabatic_list**2 + 4 * V12_diabatic_list * V21_diabatic_list - 2 * V11_diabatic_list * V22_diabatic_list + V22_diabatic_list**2))
	# 1/2 (V11 + V22 + Sqrt[V11^2 + 4 V12 V21 - 2 V11 V22 + V22^2])
	V22_adiabatic_list = (1/2) * (V11_diabatic_list + V22_diabatic_list + np.sqrt(V11_diabatic_list**2 + 4 * V12_diabatic_list * V21_diabatic_list - 2 * V11_diabatic_list * V22_diabatic_list + V22_diabatic_list**2))

	### plot V11 & V22 ###
	ax.plot(x_list, V11_adiabatic_list, color='C0', linestyle=linestyle, linewidth=2.0, label=r'$u_{1}, c = $' + f'{c:.2f}')
	ax.plot(x_list, V22_adiabatic_list, color='C1', linestyle=linestyle, linewidth=2.0, label=r'$u_{2}, c = $' + f'{c:.2f}')
	ax.set_xlabel(r'$x$ [A]')
	ax.set_ylabel(r'[eV]')

### legend & save file ###
plt.legend()
plt.tight_layout()
filename = f'figures/adiabatic_task4.pdf'
plt.savefig(filename)
filename = f'figures/adiabatic_task4.png'
plt.savefig(filename)
plt.show()

