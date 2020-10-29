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

lambda_list = np.linspace(0.0, 1.0, 50)

def inequality_holds(x, y):
	for lambd in lambda_list:
		if ((1 - x**(2 * (1 - lambd))) * (1 - y**(2 * lambd)) > (1 - 
   x**(2 * (1 - lambd) * y**(2 * lambd)))):
			return False
	return True

t = np.linspace(0.0, 1.0, 40)

(X, Y) = np.meshgrid(t, t)
for x in X.flatten():
	for y in Y.flatten():
		if not inequality_holds(x, y):
			x = np.nan


fig, ax = plt.subplots(figsize=(6, 6))
ax.scatter(X, Y, marker='s', label='equality holds')
ax.set_xlabel(r'$|w_1|$')
ax.set_ylabel(r'$|w_2|$')

### legend & save file ###
plt.tight_layout()
plt.legend()
filename = f'inequality_plot.pdf'
plt.savefig(filename)
plt.show()

