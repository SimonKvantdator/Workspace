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

t = np.linspace(-0.2, 1.0, 400)
z1 = t
z2_boundary1 = 1.0 * np.ones(t.size)
z2_boundary2 = 1.0 / (2.0 * np.abs(z1))
# z2_boundary3 = 

z2 = [z2 for z2 in np.minimum(z2_boundary1, z2_boundary2)]

fig, ax = plt.subplots(figsize=(4, 4))
ax.fill_between(z1, z2, y2=-0.2, facecolor='teal', edgecolor='black', linestyle='--', linewidth=1.5)
ax.set_xlim(left=0.0, right=1.1)
ax.set_ylim(bottom=0.0, top=1.1)
ax.set_xticks([0.0, 0.5, 1.0])
ax.set_yticks([0.0, 0.5, 1.0])
ax.set_xticklabels([r'$0$', r'$\frac{1}{2}$', r'$1$'])
ax.set_yticklabels([r'$0$', r'$\frac{1}{2}$', r'$1$'])
ax.set_xlabel(r'$|z_1|$')
ax.set_ylabel(r'$|z_2|$')

### legend & save file ###
plt.tight_layout()
filename = f'domain_plot.pdf'
plt.savefig(filename)
plt.show()
