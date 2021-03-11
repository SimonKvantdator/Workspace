import numpy as np
import matplotlib as mpl
from matplotlib import pyplot as plt

y = np.array([.1, .2, .6, .7])

# This is what we want to maximize
def ratio_between_means(treshold):

    # Make sure input is reasonable
    if (treshold < 0. or treshold > 1.):
        return 0

    y_above = y[y > treshold]
    y_below = y[y <= treshold]

    # Make sure there are points both below and above treshold
    if (not len(y_below) or not len(y_above)):
        return 0

    # Return ratio of means above and below treshold, with a minus sign since we have a minimizer tool and we want to maximize it
    return np.mean(y_above) / np.mean(y_below)

# Calculate ratio for each treshold between 0 and 1
treshold_list = np.linspace(0.0, 1.0, num=1000)
ratio_list = [ratio_between_means(treshold) for treshold in treshold_list]

# Plot ratio as a function of treshold
fig, ax = plt.subplots(figsize=(4, 3))
ax.plot(treshold_list, ratio_list)
ax.set_xlabel('treshold')
ax.set_ylabel('ratio of mean above and mean below')

plt.tight_layout()
plt.grid()
plt.show()
