# plot h(t)
# Created by Martin Gren 2014-10-25.

# imports
import matplotlib.pyplot as plt
import numpy as np


# input file
filename1 = 'trajactory1.dat'
filename2 = 'trajactory2.dat'
filename3 = 'trajactory3.dat'
filename4 = 'trajactory4.dat'
filename5 = 'trajactory5.dat'

# import data
data1 = np.loadtxt(filename1)
data2 = np.loadtxt(filename2)
data3 = np.loadtxt(filename3)
data4 = np.loadtxt(filename4)
data5 = np.loadtxt(filename5)

# size of figure
plt.figure(figsize=(8,4.5))

# plot
plt.plot(data1[:,0], data1[:,2],'-')
plt.plot(data2[:,0], data2[:,2],'-')
plt.plot(data3[:,0], data3[:,2],'-')
plt.plot(data4[:,0], data4[:,2],'-')
plt.plot(data5[:,0], data5[:,2],'-')



# labels
plt.xlabel('t / [ms]', fontsize=12)
plt.ylabel(' v / [\mu m / ms]', fontsize=12)

# set tick fontsize
plt.yticks(fontsize=12)
plt.xticks(fontsize=12)

# axis limits
plt.xlim([0,0.5])

# save the plot
plt.savefig('velocity.pdf')

# display the plot
plt.show()

