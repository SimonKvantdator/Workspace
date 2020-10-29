# A beam of electrons with energies of 250 MeV is scattered through an angle 
# of 10 degrees by a heavy nucleus. It is found that the differential cross-
# section is 65% of that expected from scattering from a point nucleus. 
# Estimate the root mean square radius of the nucleus.
import numpy as np
from scipy.optimize import fsolve

electron_mass = 0.510998950 # [MeV / c^2]
hbar = 197.3269804 # [MeV fm / c]

scattering_angle = 10. * (np.pi / 180) # [rad]
kinetic_energy = 250. # [MeV]
Z = 1.0 # [e] placeholder since it will cancel out

p = kinetic_energy # [MeV / c]
q = 2.0 * p * np.sin(scattering_angle / 2.0) # [MeV / c]

def nucleus_charge_density(r, R): # homogenously charged ball with radius R
	return Z / (4.0 * np.pi * R**3 / 3.0) * (0.0 <= r) * (r <= R) # [e / fm^3]
	
def form_factor(R):
	R = R[0]
	if R > 0.0:
		r = np.linspace(0.0, R, 2000)

		form_factor = (4.0 *  np.pi * hbar ) / (Z * q) * \
			np.trapz(r * nucleus_charge_density(r, R) * np.sin(q * r / hbar), x=r)
		# [((MeV fm / c) / (e MeV / c)) (fm e / fm^3) fm] = [1]

		return form_factor 
	else:
		return 1.0

def objective_function(R):
	return form_factor(R)**2 - 0.65

# calculate max radius of nucleus
R_max = fsolve(objective_function, [2.0])[0]
print(f'R_max = {R_max} [fm]')

# calculate RMS radius of nucleus
r = np.linspace(0.0, R_max, 2000)
jacobian = 4.0 * np.pi * r**2
R_rms = np.sqrt(np.trapz(r**2 * nucleus_charge_density(r, R_max) / Z * jacobian, x=r))
print(f'R_rms = {R_rms} [fm]')
print('or')
print(f'R_rms = {np.sqrt((1.0 - np.sqrt(0.65)) * 6.0 * hbar**2 / q**2)} [fm]')
