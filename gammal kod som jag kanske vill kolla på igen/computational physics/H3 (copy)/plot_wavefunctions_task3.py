import numpy as np
import matplotlib as mpl
from matplotlib import pyplot as plt
import struct

# plot integrated probabilities?
plot_integrated_probabilities = 0

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
d = 0.5 # wavepacket width [Å]

def psi_p(p):
    return (d**2 / (np.pi * hbar**2))**(1 / 4) * \
		np.exp(-d**2 * (p - p0)**2 / (2 * hbar**2)) * \
		np.exp(-1j * p * x0 / hbar)

alphas = [0.5, 0.5, 0.5, 2.0, 2.0, 2.0]
T0s = [0.08, 0.10, 0.12, 0.08, 0.10, 0.12]
for (alpha, T0) in zip(alphas, T0s):
	p0 = np.sqrt(2 * hydrogen_mass * T0) # [eV fs / Å] mean momentum

	### read binary files generated by C ###
	x_list = np.fromfile(f'data/x_list_alpha={alpha:.1f}_T0={T0:.2f}_task3.bin')
	p_list = np.fromfile(f'data/p_list_alpha={alpha:.1f}_T0={T0:.2f}_task3.bin')
	real_psi_x_list = np.fromfile(f'data/real_psi_x_list_alpha={alpha:.1f}_T0={T0:.2f}_task3.bin') # real_psi_x_list[x_index + x_list_length * time_index], [Å^(-1/2)]
	imag_psi_x_list = np.fromfile(f'data/imag_psi_x_list_alpha={alpha:.1f}_T0={T0:.2f}_task3.bin')
	real_psi_p_list = np.fromfile(f'data/real_psi_p_list_alpha={alpha:.1f}_T0={T0:.2f}_task3.bin')
	imag_psi_p_list = np.fromfile(f'data/imag_psi_p_list_alpha={alpha:.1f}_T0={T0:.2f}_task3.bin')
	parameters = np.fromfile(f'data/parameters_task3.bin') # {x0, x_max, x_list_length, timestep, nbr_timesteps, p0, d, alpha}

	### extract parameters ###
	x_list_length = int(parameters[2])
	timestep = parameters[3]
	nbr_timesteps = int(parameters[4])
	

	### sort p_lists ###
	idx   = np.argsort(p_list)
	p_list = np.array(p_list)[idx]

	### extract probability densities ###
	probs_x = real_psi_x_list**2 + imag_psi_x_list**2
	probs_p = real_psi_p_list**2 + imag_psi_p_list**2

	### plot probability densities ###
	fig, (ax1, ax2) = plt.subplots(nrows=1, ncols=2, figsize=(8,4), sharey=True)
	skip = 10 # for not as packed plotting
	time_factor = 0.1
	for time_index in range(0, nbr_timesteps, skip):
		# extract probability density for current time
		current_probs_x = probs_x[x_list_length * time_index:x_list_length * (time_index + 1)]
		current_probs_p = probs_p[x_list_length * time_index:x_list_length * (time_index + 1)]
		current_probs_p = np.array(current_probs_p)[idx] # sort according to p_list sorting

		# normalization doesn't matter and this looks better
		factor_x = 40.0 * timestep
		factor_p = 90.0 * timestep

		# actually plot
		ax1.fill_between(x_list, -factor_x * current_probs_x + timestep * time_index, y2=timestep * time_index, color='white', zorder=2 * time_index)
		ax1.plot(x_list, -factor_x * current_probs_x + timestep * time_index, color='black', zorder=2 * time_index + 1)
		ax2.fill_between(p_list, -factor_p * current_probs_p + timestep * time_index, y2=timestep * time_index, color='white', zorder=2 * time_index)
		ax2.plot(p_list, -factor_p * current_probs_p + timestep * time_index, color='black', zorder=2 * time_index + 1)

	ax1.set_xlabel(r'$x$ [Å]')
	ax1.set_ylabel(r'$t$ [fs]')
	ax1.set_xlim([-7, 7])
	ax1.set_ylim([timestep * nbr_timesteps, -43]) # note the reverse order
	ax2.set_xlabel(r'$p$ [eV fs / Å]')
	ax2.set_xlim([-10.0, 10.0])

	### legend & save file ###
	plt.tight_layout()
	filename = f'figures/alpha={alpha:.1f}_p0={p0:.1f}'.replace('.',',') + '_task3.pdf'
	plt.savefig(filename)
	filename = f'figures/alpha={alpha:.1f}_p0={p0:.1f}'.replace('.',',') + '_task3.png'
	plt.savefig(filename)
	plt.show()


	### determine integrated probability densities ###
	prob_less_than_0 = np.zeros(nbr_timesteps) # initiate
	for time_index in range(0, nbr_timesteps):
		current_probs_x = probs_x[x_list_length * time_index:x_list_length * (time_index + 1)]
		prob_less_than_0[time_index] = np.trapz(current_probs_x[x_list < 0], x_list[x_list < 0])

	### plot integrated probability density ###
	t = timestep * np.array([i for i in range(nbr_timesteps)])
	if plot_integrated_probabilities:
		fig, ax = plt.subplots(nrows=1, ncols=1, figsize=(6,4))
		ax.plot(t, prob_less_than_0)
		ax.set_xlabel(r'$t$ [fs]')
		ax.set_ylabel(r'$P(x<0)$')
		ax.set_ylim([0.0, 1.05])

		### legend & save file ###
		plt.tight_layout()
		filename = f'figures/prob_less_than_0_alpha={alpha:.1f}_p0={p0:.1f}'.replace('.',',') + '_task3.pdf'
		plt.savefig(filename)
		plt.show()

	print(f'for alpha = {alpha:.1f} and p0^2/2m = {T0:.2f}, ')
	print(f'P(x<0) = {prob_less_than_0[-1]:.6f} @ t=inf')