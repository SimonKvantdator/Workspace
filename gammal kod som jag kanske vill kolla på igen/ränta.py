import numpy as np

# 7% increase in a year
k = np.log(1.07)  # [year⁻¹]

# Add 1000 kr each month for 20 years
s = 0
dt = 1 / 12.  # [year]
for i in range(20*12):
    s += 1000  # [kr]
    s *= np.exp(k * dt)

print(s)
