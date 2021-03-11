import numpy as np

initial_mass = 1e-6 # [g]
half_life = 15. # [h]
molar_mass = 23.990962782 # [g / mol] (taken from WolframAlpha)

def amount_of_24Na(t): # t in unit [h]
	return initial_mass * (1. / 2.)**(t / half_life) # [g]

amount_of_decayed_24Na = amount_of_24Na(0) - amount_of_24Na(1) # [g]
nbr_of_decayed_24Na = amount_of_decayed_24Na / molar_mass # [mol]

# decay mode is beta-minus so one electron for each atom of 24Na that has decayed
nbr_electrons_emitted = nbr_of_decayed_24Na
print(f'{nbr_electrons_emitted:.1e} mol electrons emitted')
