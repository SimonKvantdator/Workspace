# plot h(t)
# Created by Martin Gren 2014-10-25.

# imports
import matplotlib.pyplot as plt
import numpy as np


# input file
filename = 'hartee.dat'


# import data
data = np.loadtxt(filename)


# size of figure
plt.figure(figsize=(8, 4.5))

# plot
plt.plot(data[:,0], data[:,1],'-')
r = data[:,0]

#hydrogen #
#f =2 *r * np.exp(-r)
#plt.plot(r,f, '--')

#Hartee #
V_H = 1 / r - (1 + 1 / r) * np.exp(-2 * r)
plt.plot(r, V_H, '--')

# labels
plt.xlabel(r'$r$ [a.u.]', fontsize=12)
plt.ylabel(r' $V_h$ [hartee]', fontsize=12)

# set tick fontsize
plt.yticks(fontsize=12)
plt.xticks(fontsize=12)

# axis limits
#plt.xlim([0,10])

# save the plot
plt.savefig('position.pdf')

# display the plot
plt.show()

