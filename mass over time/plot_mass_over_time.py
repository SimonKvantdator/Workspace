# Calculations
import numpy as np
import GPy

# Data representation
import matplotlib as mpl
import matplotlib.pyplot as plt
import pandas as pd

from datetime import datetime


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
data = pd.read_csv('mass_data.csv')


# Some functions for data wrangling
def parse_date(d):
    return datetime.strptime(d, "%Y-%m-%d")


def days_since_first_date(d):
    d0 = parse_date(data['date'].values[0])

    return (d - d0).days


# Get mass and time values from data
t = np.array([days_since_first_date(parse_date(d)) for d in data['date'].values])  # [days]
m = data['mass'].values  # [kg]

# Perform Gaussian process regression
input_dim = 1
k = GPy.kern.RBF(input_dim, variance=2000, lengthscale=10)
t = t.reshape((-1, 1))
m = m.reshape((-1, 1))
model = GPy.models.GPRegression(t, m, k)
model.optimize()

print(model)
print('\n' + f'log(likelihood(data)) = {model.log_likelihood():.2f}')

# Plot mass over time
fig, ax = plt.subplots(figsize=(6, 4))
model.plot(ax=ax)  # GPy's plotting function is actually pretty handy
ax.set_xlabel(r'time [days since first measurement]')
plt.xticks(rotation=90)
ax.set_ylabel(r'mass [kg]')

# Save and show plot
plt.tight_layout()
filename = 'mass_over_time.pdf'
plt.savefig(filename)
plt.show()
