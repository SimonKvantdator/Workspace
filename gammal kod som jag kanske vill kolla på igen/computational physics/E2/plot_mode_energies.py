# plot the energies
# Created by Martin Gren 2014-10-25.

# imports
import matplotlib.pylab as plt
import numpy as np

# input file
filename = 'mode_energies.dat'

# import data
data = np.loadtxt(filename)

# size of figure
plt.figure(figsize=(8,6))

# plot (using only 1000 points)
skip = int(len(data[:,0]) / 1000) + 1
plt.plot(data[0::skip,0], data[0::skip,1], '-')
plt.plot(data[0::skip,0], data[0::skip,2], '-')
plt.plot(data[0::skip,0], data[0::skip,3], '-')
plt.plot(data[0::skip,0], data[0::skip,4], '-')
plt.plot(data[0::skip,0], data[0::skip,5], '-')

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
plt.savefig('mode_energies.pdf')
plt.show()
