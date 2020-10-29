import numpy as np
import matplotlib as mpl
from matplotlib import pyplot as plt
import struct

### for plotting nice plots ###
width = 6
height = width / 1.5
fsize = 16
font = {'size': fsize}
mpl.rc('font', **font)
mpl.rc('xtick', labelsize=fsize)
mpl.rc('ytick', labelsize=fsize)
mpl.rc('text', usetex=False)

### write down Eckart potential ###
def eckart(x, V0=0.1, alpha=0.5):
    return V0 / np.cosh(x / alpha)**2


### plot eckart potential for different alpha:s ###
alpha1 = 0.5
alpha2 = 2.0
x = np.linspace(-7.0, 7.0, num=300)

fig, ax = plt.subplots(nrows=1, ncols=1, figsize=(6,4))
ax.plot(x, eckart(x, alpha=alpha1), label=r'$\alpha = $' + f'{alpha1:.1f}')
ax.plot(x, eckart(x, alpha=alpha2), label=r'$\alpha = $' + f'{alpha2:.1f}')
ax.set_xlabel(r'$x$ [A]')
ax.set_ylabel(r'$V(x)$ [eV]')

### legend & save file ###
ax.legend()
plt.tight_layout()
filename = f'figures/eckart_barrier_task3.pdf'
plt.savefig(filename)
plt.show()

