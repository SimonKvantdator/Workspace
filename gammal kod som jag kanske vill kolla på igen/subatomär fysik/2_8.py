import numpy as np

La138_proportion = 0.09 / 100
La_atomic_weight = 138.91 # [u]
beta_emitted = 7.8e2 # [1 / s kg]
gamma_emitted = beta_emitted / 2.0 # [1 / s kg]
beta_decays = beta_emitted + gamma_emitted # [1 / s kg]
gamma_decays = gamma_emitted # [1 / s kg]

La_atomic_weight = La_atomic_weight * 1.66053907e-27 # [kg]
mean_lifetime = 1.0 / ((beta_decays + gamma_decays) * La_atomic_weight) # [s]

print(f'mean_lifetime = {mean_lifetime} [s]')