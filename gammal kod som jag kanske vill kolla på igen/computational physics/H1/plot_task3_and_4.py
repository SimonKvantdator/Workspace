import numpy as np
import matplotlib as mpl
from matplotlib import pyplot as plt
import struct

### for plotting nice plots ###
width = 6
height = width / 1.5
fsize = 16
font = {'size': fsize}    
mpl.rc('font', **font)
mpl.rc('xtick', labelsize=fsize)
mpl.rc('ytick', labelsize=fsize)
mpl.rc('text', usetex=False)

### read binary files generated by C ###
temperatures_solid = np.fromfile('temperatures_task3.bin')
pressures_solid = np.fromfile('pressures_task3.bin')
parameters_solid = np.fromfile('parameters_task3.bin')
temperatures_liquid = np.fromfile('temperatures_task4.bin')
pressures_liquid = np.fromfile('pressures_task4.bin')
parameters_liquid = np.fromfile('parameters_task4.bin')
indices_solid = np.array([i for i in range(len(temperatures_solid))])
indices_liquid = np.array([i for i in range(len(temperatures_liquid))])

### extract parameters ###
nbr_timesteps_per_epoch = int(parameters_solid[0])
nbr_epochs = int(parameters_solid[1])
T_decay_constant = parameters_solid[2]
P_decay_constant = parameters_solid[3]
timestep = parameters_solid[5]

### plot T(t) & P(t) ###
fig, (ax1, ax2) = plt.subplots(nrows=2, figsize=(8, 8))
ax1.plot(timestep * indices_solid, temperatures_solid, label=r'solid phase', color='C0')
ax1.plot(timestep * indices_liquid, temperatures_liquid, label=r'liquid phase', color='C1')
ax1.set_xlabel(r'$t$ [ps]')
ax1.set_ylabel(r'$T$ [K]')
ax2.plot(timestep * indices_solid, pressures_solid, label=r'solid phase', color='C0')
ax2.plot(timestep * indices_liquid, pressures_liquid, label=r'liquid phase', color='C1')
ax2.set_xlabel(r'$t$ [ps]')
ax2.set_ylabel(r'$P$ [eV / A$^3$]')

### plot 500 C line ###
T_eq_solid = 500 + 273.15
P_eq_solid = 6.242e-7
T_eq_liquid = 700 + 273.15
P_eq_liquid = 6.242e-7
ax1.plot(timestep * indices_solid, T_eq_solid * np.ones(indices_solid.shape), '--', zorder=-1, label=r'500 $^\mathrm{o}$C', color='k')
ax1.plot(timestep * indices_liquid, T_eq_liquid * np.ones(indices_liquid.shape), ':', zorder=-1, label=r'700 $^\mathrm{o}$C', color='k')
ax2.plot(timestep * indices_solid, P_eq_solid * np.ones(indices_solid.shape), '--', zorder=-1, label=r'1 bar', color='k')

### legend & save file ###
ax1.legend(loc=1)
ax2.legend()
plt.tight_layout()
filename = f'plot_task3_and_4.pdf'
plt.savefig(filename)
plt.show()