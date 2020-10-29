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

t = np.linspace(-0.2, 1.0, 200)
epsilon = 0.25
z1 = t
z2a = z1
z2b = np.sqrt((epsilon**2 - t**2).clip(min=0)) # Negative/imaginary numbers are treated as 0
z2 = [z2 for z2 in np.maximum(z2a, z2b)]

# plot Hartogs triangle
fig, ax = plt.subplots(figsize=(4, 4))
ax.fill_between(z1, z2, y2=-0.2, facecolor='teal', edgecolor='black', linestyle='--', linewidth=1.5)
ax.set_xlim(left=0.0, right=1.1)
ax.set_ylim(bottom=0.0, top=1.1)

# draw p
p_coords = [0.3, 0.8]
ax.scatter(p_coords[0], p_coords[1], marker='o', color='black')
ax.text(p_coords[0] + 0.03, p_coords[1] + 0.01, r'$p$')

# draw Hartogs figure
a = 0.93
b = 0.1
H1_xcoords = [0.0, 0.0, a, a,    1.0]
H1_ycoords = [0.0, 0.90, 0.90, 0.90, 0.90]
H1 = plt.fill_between(H1_xcoords, H1_ycoords, color='blue', alpha=0.3)
ax.text(H1_xcoords[2] - 0.1, H1_ycoords[2] + 0.05, r'$H^\prime$', color='blue')

# add axis labels
ax.set_xticks([0.0, 0.5, a, 1.0])
ax.set_yticks([0.0, b, epsilon, 0.5, 1.0])
ax.set_xticklabels([r'$0$', r'$\frac{1}{2}$', r'$a$', r'$1$'])
ax.set_yticklabels([r'$0$', r'$b$', r'$\epsilon$', r'$\frac{1}{2}$', r'$1$'])
ax.set_xlabel(r'$|z_1|$')
ax.set_ylabel(r'$|z_2|$')



### legend & save file ###
plt.tight_layout()
filename = f'augmented_hartog_triangle_with_augmented_hartog_figure.pdf'
plt.savefig(filename)
plt.show()
