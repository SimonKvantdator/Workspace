# plot the displacements
# Created by Martin Gren 2014-10-25.

# imports
import matplotlib.pylab as plt
import numpy as np

# input file
filename = 'displacements.dat'

# import data
data = np.loadtxt(filename)

# size of figure
plt.figure(figsize=(8,6))

# plot (using only 1000 points)
# skip = int(len(data[:,0]) / 1000) + 1
# plt.plot(data[0::skip,0], data[0::skip,1], '-', label=r'')
# plt.plot(data[0::skip,0], data[0::skip,2], '-', label=r'Atom 1')
# plt.plot(data[0::skip,0], data[0::skip,3], '-', label=r'Atom 2')
# plt.plot(data[0::skip,0], data[0::skip,4], '-', label=r'Atom 3')
# plt.plot(data[0::skip,0], data[0::skip,5], '-', label=r'')
plt.plot(data[:, 0], data[:, 1], '-', label=r'')
plt.plot(data[:, 0], data[:, 2], '-', label=r'Atom 1')
plt.plot(data[:, 0], data[:, 3], '-', label=r'Atom 2')
plt.plot(data[:, 0], data[:, 4], '-', label=r'Atom 3')
plt.plot(data[:, 0], data[:, 5], '-', label=r'')

# labels
plt.xlabel('Time / [dim. unit]', fontsize=12)
plt.ylabel('Displacement / [dim. unit]', fontsize=12)

# legend
plt.legend()
leg = plt.gca().get_legend()
ltext  = leg.get_texts()
plt.setp(ltext, fontsize=12) 

# tick fontsize
plt.xticks(fontsize=12)
plt.yticks(fontsize=12)

# save and display the plot
plt.savefig('displacements.pdf')
plt.show()
