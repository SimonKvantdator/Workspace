# plot the powerspectrum
# Created by Martin Gren 2014-10-25.

# imports
import matplotlib.pylab as plt
import numpy as np

# input file
filename = '../Task6/powerspectrums.dat'

# import data
data = np.loadtxt(filename)

# size of figure
plt.figure(figsize=(8,6))

# plot
plt.plot(data[:,0], data[:,1], '-', label=r'$q1$')
plt.plot(data[:,0], data[:,2], '--', label=r'$q2$')
plt.plot(data[:,0], data[:,3], ':', label=r'$q3$')

# labels
plt.xlabel('Frequency / [arb. unit]', fontsize=12)
plt.ylabel('Power spectrum / [arb. unit]', fontsize=12)

# bodge
plt.xlim(left=-1.0, right=1.0)

# set tick fontsize
plt.yticks(fontsize=12)
plt.xticks(fontsize=12)

# save and display the plot
plt.legend(fontsize=12)
plt.tight_layout()
plt.savefig('../Task6/powerspectrum.pdf')
plt.show()
