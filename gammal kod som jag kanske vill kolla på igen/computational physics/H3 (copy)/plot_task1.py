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

### write down exact Fourier transform ###
hbar = 0.658212 # [eV fs]
hydrogen_mass = 104.5 # [eV fs^2 / A^2]
x0 = 0.0 # mean position
p0 = np.sqrt(2 * hydrogen_mass * 0.1) # [eV fs / Å] mean momentum obtained from solving p0^2 / 2m = 0.1 eV
d = 0.5 # wavepacket width [Å]
def psi_p(p):
    return (d**2 / (np.pi * hbar**2))**(1 / 4) * \
		np.exp(-d**2 * (p - p0)**2 / (2 * hbar**2)) * \
		np.exp(-1j * p * x0 / hbar)

### read binary files generated by C ###
x_list = np.fromfile("data/x_list_task1.bin")
p_list = np.fromfile("data/p_list_task1.bin")
real_psi_x_list = np.fromfile("data/real_psi_x_list_task1.bin")
imag_psi_x_list = np.fromfile("data/imag_psi_x_list_task1.bin")
real_psi_p_list = np.fromfile("data/real_psi_p_list_task1.bin")
imag_psi_p_list = np.fromfile("data/imag_psi_p_list_task1.bin")

### sort p_lists ###
idx   = np.argsort(p_list)
p_list = np.array(p_list)[idx]
real_psi_p_list = np.array(real_psi_p_list)[idx]
imag_psi_p_list = np.array(imag_psi_p_list)[idx]


### plot probability densities ###
fig, (ax1, ax2) = plt.subplots(nrows=1, ncols=2, figsize=(8,4), sharey=False)
ax1.plot(x_list, real_psi_x_list**2 + imag_psi_x_list**2, color='C0')
ax1.set_xlabel(r'$x$ [Å]')
ax1.set_ylabel(r'$|\psi|^2$')
ax1.set_xlim([-2.5, 2.5])
ax2.plot(p_list, real_psi_p_list**2 + imag_psi_p_list**2, '.', color='C1', label=r'FFT')
ax2.plot(p_list, np.abs(psi_p(p_list))**2, '-', color='C2', label=r'analytical')
ax2.axvline(p0, label=r'$p_0$', linestyle='--')
ax2.set_xlabel(r'$p$ [eV fs / Å]')
ax2.set_xlim([-1.0, 23.0])

### legend & save file ###
ax2.legend()
plt.tight_layout()
filename = f'figures/task1.pdf'
plt.savefig(filename)
filename = f'figures/task1.png'
plt.savefig(filename)
plt.show()

### reality check ###
print(f'||psi(x)|| = {np.trapz(real_psi_x_list**2 + imag_psi_x_list**2, x_list):.2f}')
print(f'||F(psi)(p)|| = {np.trapz(np.abs(psi_p(p_list))**2, p_list):.2f} from analytic expression')
print(f'||F(psi)(p)|| = {np.abs(np.trapz(real_psi_p_list**2 + imag_psi_p_list**2, p_list)):.2f} from FFT')
