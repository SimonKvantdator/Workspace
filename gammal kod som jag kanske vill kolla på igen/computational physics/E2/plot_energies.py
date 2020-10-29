# plot the energies
# Created by Martin Gren 2014-10-25.

# imports
import matplotlib.pylab as plt
import numpy as np

# input file
filename = 'energies.dat'

# import data
data = np.loadtxt(filename)

# size of figure
plt.figure(figsize=(8,6))

# plot (using only 1000 points)
skip = int(len(data[:,0]) / 1000) + 1
plt.plot(data[0::skip,0], data[0::skip,1], '-', label=r'Potential energy')
plt.plot(data[0::skip,0], data[0::skip,2], '-', label=r'Kinetic energy')
plt.plot(data[0::skip,0], data[0::skip,3], '-', label=r'Total energy')

# labels
plt.xlabel('Time / [dim. unit]', fontsize=12)
plt.ylabel('Energy / [dim. unit]', fontsize=12)

# legend
plt.legend()
leg = plt.gca().get_legend()
ltext  = leg.get_texts()
plt.setp(ltext, fontsize=12) 

# tick fontsize
plt.xticks(fontsize=12)
plt.yticks(fontsize=12)

# save and display the plot
plt.tight_layout()
plt.savefig('energies.pdf')
plt.show()
