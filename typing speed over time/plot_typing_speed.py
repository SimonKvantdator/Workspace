# Calculations
import numpy as np
import GPy

# Data representation
import matplotlib as mpl
import matplotlib.pyplot as plt
import json


# Settings for plotting nice plots
width = 6
height = width / 1.5
fsize = 16
font = {'size': fsize}
mpl.rc('font', **font)
mpl.rc('xtick', labelsize=fsize)
mpl.rc('ytick', labelsize=fsize)
mpl.rc('text', usetex=True)

# Import mass data
with open('typing-data.json', 'r') as myfile:
    data = json.loads(myfile.read())


# Get mass and time values from data
nbr_sessions = len(data)
session_idxs = np.array([i for i in range(0, nbr_sessions)])
speeds = np.array([float(data[i]['errors']) for i in range(0, nbr_sessions)])

# Perform Gaussian process regression
input_dim = 1
k = GPy.kern.RBF(input_dim, variance=200, lengthscale=100)
x = session_idxs.reshape((-1, 1))
y = speeds.reshape((-1, 1))
model = GPy.models.GPRegression(x, y, k)
model.optimize()

print(model)
print('\n' + f'log(likelihood(data)) = {model.log_likelihood():.2f}')

# Plot mass over time
fig, ax = plt.subplots(figsize=(6, 4))
model.plot(ax=ax)  # GPy's plotting function is actually pretty handy
ax.set_xlabel(r'session number')
ax.set_xlim(left=-10)
plt.xticks(rotation=90)
ax.set_ylabel(r'typing speed [chars/m]')

# Save and show plot
plt.tight_layout()
filename = 'typing_speed_over_time.pdf'
plt.savefig(filename)
plt.show()
