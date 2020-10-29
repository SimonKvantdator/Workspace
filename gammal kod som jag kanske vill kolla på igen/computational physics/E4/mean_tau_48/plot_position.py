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
filename6 = 'mean_trajactory.dat'

# import data
data1 = np.loadtxt(filename1)
data2 = np.loadtxt(filename2)
data3 = np.loadtxt(filename3)
data4 = np.loadtxt(filename4)
data5 = np.loadtxt(filename5)
data6 = np.loadtxt(filename6)

# size of figure
plt.figure(figsize=(8,4.5))

# plot
skip = int(len(data1[:,0])/100) + 1
plt.plot(data1[::skip,0], data1[::skip,1],'-')
plt.plot(data2[::skip,0], data2[::skip,1],'-')
plt.plot(data3[::skip,0], data3[::skip,1],'-')
plt.plot(data4[::skip,0], data4[::skip,1],'-')
plt.plot(data5[::skip,0], data5[::skip,1],'-')

plt.plot(data6[::skip,0], data6[::skip,1], color='k', linewidth=2.0, linestyle='-')
plt.plot(data6[::skip,0], data6[::skip,1] + data6[::skip,2], color='k', linewidth=2.0, linestyle='--')
plt.plot(data6[::skip,0], data6[::skip,1] - data6[::skip,2], color='k', linewidth=2.0, linestyle='--')


# labels
plt.xlabel('t / [ms]', fontsize=12)
plt.ylabel(' x / [\mu m]', fontsize=12)

# set tick fontsize
plt.yticks(fontsize=12)
plt.xticks(fontsize=12)

# axis limits
#plt.xlim([0,10])

# save the plot
plt.savefig('position.pdf')

# display the plot
plt.show()

