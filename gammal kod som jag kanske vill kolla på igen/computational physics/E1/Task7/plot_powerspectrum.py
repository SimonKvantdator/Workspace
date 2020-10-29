# plot the powerspectrum
# Created by Martin Gren 2014-10-25.

# imports
import matplotlib.pylab as plt
import numpy as np

# input file
filename = 'powerspectrums.dat'

# import data
data = np.loadtxt(filename)

# size of figure
plt.figure(figsize=(8,6))

# plot
indicies = [(f < 150. and f > -150.) for f in data[:,0]]
plt.plot(data[indicies, 0], data[indicies, 1], '-', label=r'Atom 1')
plt.plot(data[indicies, 0], data[indicies, 2], '--', label=r'Atom 2')
plt.plot(data[indicies, 0], data[indicies, 3], ':', label=r'Atom 3')

# labels
plt.xlabel('Frequency / [arb. unit]', fontsize=12)
plt.ylabel('Power spectrum / [arb. unit]', fontsize=12)

# bodge
#plt.xlim(left=-0.4, right=0.4)

# set tick fontsize
plt.yticks(fontsize=12)
plt.xticks(fontsize=12)

# save and display the plot
plt.legend(fontsize=12)
plt.tight_layout()
plt.savefig('powerspectrum.pdf')
plt.show()
