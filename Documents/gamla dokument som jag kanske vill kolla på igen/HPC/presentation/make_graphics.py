import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt

### for plotting nice plots ###
width = 6
height = 4
fsize = 20
font = {'size': fsize}
mpl.rc('font', **font)
mpl.rc('xtick', labelsize=fsize)
mpl.rc('ytick', labelsize=fsize)
mpl.rc('text', usetex=True)

# Along 3-vector
forloop = np.array([
    0.00689,
    0.00689,
    0.00688,
    0.00688,
    0.00689,
    0.00688,
    0.00688,
    0.00688,
    0.00689,
    0.00687])  # [us / iteration]

sub2_mul2 = np.array([
    0.01869,
    0.01766,
    0.01661,
    0.01661,
    0.01661,
    0.01662,
    0.01661,
    0.01662,
    0.01661,
    0.01661])  # [us / iteration]

sub4_mul4 = np.array([
    0.01659,
    0.01658,
    0.01659,
    0.01659,
    0.01658,
    0.01659,
    0.01659,
    0.01659,
    0.01659,
    0.01657])  # [us / iteration]

forloop_mean = np.mean(forloop)
forloop_var = np.var(forloop)
sub2_mul2_mean = np.mean(sub2_mul2)
sub2_mul2_var = np.var(sub2_mul2)
sub4_mul4_mean = np.mean(sub4_mul4)
sub4_mul4_var = np.var(sub4_mul4)

fig, ax = plt.subplots(figsize=(8, 4))
print(np.sqrt(forloop_var))
ax.bar(1, forloop_mean, yerr=1 * np.sqrt(forloop_var), capsize=100, color='teal')
# ax.bar(2, sub2_mul2_mean, yerr=1 * np.sqrt(sub2_mul2_var), capsize=67, color='teal')
ax.bar(2, sub4_mul4_mean, yerr=1 * np.sqrt(sub4_mul4_var), capsize=100, color='teal')

ax.set_xticks([1, 2])
ax.set_xticklabels([r'for-loop', r'4 in parallell'])
ax.set_ylabel(r'$\mu$s/iteration $\pm 1 \sigma$')
plt.tight_layout()

# Along list (with 16 3-vectors)
forloop = np.array([
    0.04739,
    0.05380,
    0.04971,
    0.04681,
    0.04706,
    0.04697,
    0.04745,
    0.04709,
    0.04718,
    0.04742])  # [us / iteration]

sub4_mul4_add4_sqrf4 = np.array([
    0.03035,
    0.03049,
    0.03049,
    0.03105,
    0.03043,
    0.03047,
    0.03050,
    0.03050,
    0.03052,
    0.03040])  # [us / iteration]

sub8_mul8_add8_sqrf8 = np.array([
    0.05954,
    0.05484,
    0.05486,
    0.05957,
    0.05957,
    0.05952,
    0.05959,
    0.05959,
    0.05960,
    0.05959])  # [us / iteration]

sub16_mul16_add16_sqrf16 = np.array([
    0.04875,
    0.04875,
    0.05321,
    0.05319,
    0.05316,
    0.05321,
    0.05319,
    0.05321,
    0.05321])  # [us / iteration]

forloop_mean = np.mean(forloop)
forloop_var = np.var(forloop)
sub4_mul4_add4_sqrf4_mean = np.mean(sub4_mul4_add4_sqrf4)
sub4_mul4_add4_sqrf4_var = np.var(sub4_mul4_add4_sqrf4)
sub8_mul8_add8_sqrf8_mean = np.mean(sub8_mul8_add8_sqrf8)
sub8_mul8_add8_sqrf8_var = np.var(sub8_mul8_add8_sqrf8)
sub16_mul16_add16_sqrf16_mean = np.mean(sub16_mul16_add16_sqrf16)
sub16_mul16_add16_sqrf16_var = np.var(sub16_mul16_add16_sqrf16)

fig, ax = plt.subplots(figsize=(8, 4))
print(np.sqrt(forloop_var))
ax.bar(1, forloop_mean, yerr=1 * np.sqrt(forloop_var), capsize=50, color='teal')
ax.bar(2, sub4_mul4_add4_sqrf4_mean, yerr=1 * np.sqrt(sub4_mul4_add4_sqrf4_var), capsize=50, color='teal')
ax.bar(3, sub8_mul8_add8_sqrf8_mean, yerr=1 * np.sqrt(sub8_mul8_add8_sqrf8_var), capsize=50, color='teal')
ax.bar(4, sub16_mul16_add16_sqrf16_mean, yerr=1 * np.sqrt(sub16_mul16_add16_sqrf16_var), capsize=50, color='teal')

ax.set_xticks([1, 2, 3, 4])
ax.set_xticklabels([r'for-loop', r'4 in parallell', r'8 in parallell', r'16 in parallell'])
ax.set_ylabel(r'$\mu$s/iteration $\pm 1 \sigma$')
plt.tight_layout()
plt.show()












