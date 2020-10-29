import numpy as np
from matplotlib import pyplot as plt
from math import factorial
from scipy import integrate
from scipy import special

hbar = 1.
m = 1.
w = 1.

def E(n):
    return (2 * n + 1) * w * hbar / 2

def H_n(x, n=2):
    return special.eval_hermite(n, x)

def V(x):
    return m * w**2 * x**2 / 2

def p(x, n=2):
    return np.sqrt(2 * m * (np.abs(E(n) - V(x))))

def psi_n_wkb(x, n=2):
    if x<-a:
        return (1 / np.sqrt(p(x, n=n))) * np.exp(-integrate.quad(p, x, -a)[0])
    elif -a<x and x<a:
        return (2 / np.sqrt(p(x, n=n))) * np.sin(integrate.quad(p, x, a)[0] + np.pi / 4)
    else:
        return (1 / np.sqrt(p(x, n=n))) * np.exp(-integrate.quad(p, a, x)[0])

def psi_n_true(x, n=2):
    return (1 / np.sqrt(2**n * factorial(n))) \
        * (m * w / (np.pi * hbar))**(1/4) \
        * np.exp(-m * w * x**2 / (2 * hbar)) \
        * H_n(np.sqrt(m * w / hbar) * x, n=n)

### plot wfn ###
fig, ax = plt.subplots(nrows=2, ncols=2, figsize=(12,12))
ax = ax.flatten()

for i,n in enumerate([1, 2, 5, 10]):
    a = np.sqrt(2 * E(n) / m) / w
    x_vec = np.linspace(-2.5 * a, 2.5 * a, 400)

    psi_n_wkb_vec = np.array([psi_n_wkb(x, n=n) for x in x_vec])
    psi_n_wkb_vec = psi_n_wkb_vec / np.sqrt(integrate.trapz(psi_n_wkb_vec**2, x_vec)) # normalize
    psi_n_true_vec = np.array([psi_n_true(x, n=n) for x in x_vec])
    psi_n_true_vec = psi_n_true_vec / np.sqrt(integrate.trapz(psi_n_true_vec**2, x_vec)) # normalize

    ax[i].plot(x_vec, psi_n_wkb_vec, color='orange', label=r'WKB $\psi(x)$')
    ax[i].plot(x_vec, psi_n_true_vec, color='olive', label=r'True $\psi(x)$')
    ax[i].plot(x_vec, V(x_vec) / np.max(V(x_vec)), '--', color='crimson', label=r'$V(x)$')
    ax[i].plot(x_vec, E(n) / np.max(V(x_vec)) *np.ones_like(x_vec), ':', color='maroon', label=r'$E_n$')

    ax[i].set_xlabel(r'$x$', fontsize=16)
    ax[i].set_xticks([-2*a, -a, 0, a, 2*a])
    ax[i].set_xticklabels([r'$-2a$', r'$-a$', r'$0$', r'$a$', r'$2a$'])
    ax[i].set_yticks([])
    ax[i].set_title(f'n = {n}', fontsize=20)
    ax[i].legend(fontsize=10)
plt.tight_layout()
fig.subplots_adjust(hspace=0.3)
plt.show()