import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt

### for plotting nice plots ###
width = 6
height = width / 1.5
fsize = 16
font = {'size': fsize}
mpl.rc('font', **font)
mpl.rc('xtick', labelsize=fsize)
mpl.rc('ytick', labelsize=fsize)
mpl.rc('text', usetex=True)

alpha = 1.0 / 137.0
p = 1
theta = np.linspace(0.0, np.pi, 100)

differential_cross_section = \
    np.pi * alpha**2 / (32 * p**2) * \
    (7 + np.cos(2 * theta))**2 / \
    (-1 + np.cos(theta))**2

fig, ax = plt.subplots(figsize=(4,4))
ax.plot(theta, differential_cross_section, linewidth=2)
ax.set_xlim(left=0.0, right=np.pi)
ax.set_ylim(bottom=0.0, top=1.0e0)
ax.set_xlabel(r'$\theta$')
ax.set_ylabel(r'$\frac{\mathrm{d}\sigma}{\mathrm{d}\cos{\theta}}$')
ax.set_xticks([0.0, np.pi / 2, np.pi])
ax.set_yticks([])
ax.set_xticklabels([r'$0$', r'$\frac{\pi}{2}$', r'$\pi$'])

### save file ###
plt.tight_layout()
filename = f'plot_differential_cross_section.pdf'
plt.savefig(filename)
plt.show()