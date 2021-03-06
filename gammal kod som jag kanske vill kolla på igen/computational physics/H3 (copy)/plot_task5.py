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

### nature constants ###
hbar = 0.658212 # [eV fs]
hydrogen_mass = 104.5 # [eV fs^2 / A^2]

cs = [0.1]
T0s = [0.10]
for (c, T0) in zip(cs, T0s):

	### read binary files generated by C ###
	x_list = np.fromfile(f'data/x_list_c={c:.2f}_T0={T0:.2f}_task5.bin')
	real_psi1_x_list = np.fromfile(f'data/real_psi1_x_list_c={c:.2f}_T0={T0:.2f}_task5.bin') # real_psi_x_list[x_index + x_list_length * time_index], [Å^(-1/2)]
	real_psi2_x_list = np.fromfile(f'data/real_psi2_x_list_c={c:.2f}_T0={T0:.2f}_task5.bin')
	imag_psi1_x_list = np.fromfile(f'data/imag_psi1_x_list_c={c:.2f}_T0={T0:.2f}_task5.bin')
	imag_psi2_x_list = np.fromfile(f'data/imag_psi2_x_list_c={c:.2f}_T0={T0:.2f}_task5.bin')
	parameters = np.fromfile(f'data/parameters_task5.bin') # {x0, x_max, x_list_length, T0, c, timestep, nbr_timesteps}

	print(real_psi1_x_list)

	### extract parameters ###
	x_list_length = int(parameters[2])
	T0 = parameters[3]
	c = parameters[4]
	timestep = parameters[5]
	nbr_timesteps = int(parameters[6])

	### extract probability densities ###
	probs1_x = real_psi1_x_list**2 + imag_psi1_x_list**2
	probs2_x = real_psi2_x_list**2 + imag_psi2_x_list**2

	### plot probability densities ###
	fig, (ax1, ax2) = plt.subplots(nrows=1, ncols=2, figsize=(8,4), sharey=True)
	skip = 1 # for not as packed plotting
	time_factor = 0.1
	for time_index in range(0, nbr_timesteps, 1):
		# extract probability density for current time
		current_probs1_x = probs1_x[x_list_length * time_index:x_list_length * (time_index + 1)]
		current_probs2_x = probs2_x[x_list_length * time_index:x_list_length * (time_index + 1)]

		# normalization doesn't matter and this looks better
		factor = 6.0

		# actually plot
		ax1.fill_between(x_list, -factor * current_probs1_x + timestep * time_index, y2=timestep * time_index, color='white', zorder=2 * time_index)
		ax1.plot(x_list, -factor * current_probs1_x + timestep * time_index, color='black', zorder=2 * time_index + 1)
		ax2.fill_between(x_list, -factor * current_probs2_x + timestep * time_index, y2=timestep * time_index, color='white', zorder=2 * time_index)
		ax2.plot(x_list, -factor * current_probs2_x + timestep * time_index, color='black', zorder=2 * time_index + 1)

	ax1.set_xlabel(r'$x$ [Å]')
	ax1.set_ylabel(r'$t$ [fs]')
	ax1.set_xlim([-7.0, 7.0])
	ax1.set_ylim([timestep * nbr_timesteps, -20.0]) # note the reverse order
	ax2.set_xlabel(r'$x$ [Å]')
	ax2.set_xlim([-7.0, 7.0])

	### legend & save file ###
	plt.tight_layout()
	filename = f'figures/c={c:.2f}_T0={T0:.1f}'.replace('.',',') + '_task5.pdf'
	plt.savefig(filename)
	filename = f'figures/c={c:.2f}_T0={T0:.1f}'.replace('.',',') + '_task5.png'
	plt.savefig(filename)
	plt.show()

    ### print integrated probability for being in each state ###
	# print(f'for alpha = {alpha:.1f} and p0^2/2m = {T0:.2f}, ')
	# print(f'P(x<0) = {prob_less_than_0[-1]:.6f} @ t=inf')