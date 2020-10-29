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

r = 1.5
t = np.linspace(0.0, 1.05 * r, 3000)
z1 = t
z2 = np.sqrt(r - z1**2)

# plot {z in H_complement s.th |z| < sqrt(r)}
fig, ax = plt.subplots(figsize=(5, 5))
ax.fill_between(z1, z2, y2=-0.1,
    facecolor='teal',
    edgecolor='black',
    linestyle='--',
    linewidth=1.5,
    alpha=0.7,
    label=r'$\{\, z \in H^\mathrm{C} \textrm{ s.th. } |z|^2 < r \,\}$')
ax.set_xlim(left=0.0, right=1.3 * np.sqrt(r))
ax.set_ylim(bottom=0.0, top=1.3 * np.sqrt(r))

# plot {z in H_complement s.th |z_2| > exp(-r)}
ax.fill_between(1.5 * z1, 10 * np.ones(z1.shape), y2=np.exp(-r),
    facecolor='blue',
    edgecolor='black',
    linestyle='--',
    linewidth=1.5,
    alpha=0.6,
    label=r'$\{\, z \in H^\mathrm{C} \textrm{ s.th. } |z_2| > \mathrm{e}^{-r} \,\}$')

# draw boundary of H_complement
ax.plot(1.5 * z1, np.zeros(z1.shape),
    linestyle=(0, (3, 1, 1, 1)),
    linewidth=4,
    color='blueviolet',
    label=r'$\partial H^\mathrm{C}$')

# add axis labels
ax.set_xticks([0.0, np.sqrt(r)])
ax.set_yticks([0.0, np.exp(-r), np.sqrt(r)])
ax.set_xticklabels([r'$0$', r'$\sqrt{r}$'])
ax.set_yticklabels([r'$0$', r'$\mathrm{e}^{-r}$', r'$\sqrt{r}$'])
ax.set_xlabel(r'$|z_1|$')
ax.set_ylabel(r'$|z_2|$')



### legend & save file ###
plt.tight_layout()
plt.legend()
filename = f'A.pdf'
plt.savefig(filename)
plt.show()
