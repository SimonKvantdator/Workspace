import numpy as np
import matplotlib as mpl
from matplotlib import pyplot as plt
from scipy.optimize import minimize

x = np.linspace(0.0, 1.0, 30)
np.random.seed(42)
p = 1.4
a = 0.9
epsilon = 0.3
data = a * x**p + epsilon * (np.random.rand(x.size) - 0.5)

def prediction(args):
    A = args[0]
    P = args[1]
    return A * x**P

def objective_function(args):
    rms_error = np.trapz((prediction(args) - data)**2) / (max(x) - min(x))
    return rms_error

a_guess = 1.0
p_guess = 1.0
solution = minimize(objective_function, [a_guess, p_guess])
print(solution['x'])


fig, ax = plt.subplots(figsize=(8,4))
ax.plot(x, data, 'o')
ax.plot(x, prediction(solution['x']))

plt.tight_layout()
plt.grid()
plt.show()
