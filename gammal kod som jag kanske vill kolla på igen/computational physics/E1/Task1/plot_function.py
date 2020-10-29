# plot h(t)
# Created by Martin Gren 2014-10-25.

# imports
import matplotlib.pyplot as plt
import numpy as np


# input file
filename = 'function.dat'

# import data
data = np.loadtxt(filename)

# size of figure
plt.figure(figsize=(8,4.5))

# plot
plt.plot(data[:,0], data[:,1],'-')

# labels
plt.xlabel('t / [arb. unit]', fontsize=12)
plt.ylabel('h(t) / [arb. unit]', fontsize=12)

# set tick fontsize
plt.yticks(fontsize=12)
plt.xticks(fontsize=12)

# save the plot
plt.savefig('h_t.pdf')

# display the plot
plt.show()

