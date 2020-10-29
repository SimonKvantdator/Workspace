import numpy as np
import matplotlib.pyplot as plt
from scipy import special
from scipy import optimize

### define RHS & LHS ###
beta = -16
E = np.linspace(-1, 0, 4000)
z = beta * E
ai_0, aip_0, bi_0, bip_0 = special.airy(beta * E)
ai_1, aip_1, bi_1, bip_1 = special.airy(beta * (E + 1))
RHS = (-bi_1 * aip_0 + ai_1 * bip_0)
LHS = np.sqrt(-beta) * (-bi_1 * ai_0 + ai_1 * bi_0) * E
diff = (RHS - LHS) * np.exp(-(2/3) * z**(3/2))

### find zeros of RHS - LHS ###
indices = [] # initiate
d_last = np.nan # initiate
for i,d in enumerate(diff):
    if (d_last >= 0 and d < 0) or (d_last <= 0 and d > 0):
        indices.append(i)
    d_last = d

### plot RHS - LHS & its zeros ###
fig, ax = plt.subplots()
ax.plot(E, diff, 'b', label=r'$-Bi(1) Ai\'(0) + Ai(1) Bi\'(0) + \sqrt{-\beta} [-Bi(1) Ai(0)) + Ai(1) Bi(0)] E$')
ax.scatter(E[indices], diff[indices], marker='x', color='r')
ax.set_title(f'Nbr of zeros = {len(indices)}')
ax.set_yticks([])
plt.legend()
plt.show()