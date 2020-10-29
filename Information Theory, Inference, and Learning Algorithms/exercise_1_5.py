import numpy as np

# Parity check matrix
P = np.array([
    [1, 1, 1, 0],
    [0, 1, 1, 1],
    [1, 0, 1, 1]])
H = np.hstack((P, np.identity(3)))

# Received strings
r_list = [
    np.array([1, 1, 0, 1, 0, 1, 1]).T,
    np.array([0, 1, 1, 0, 1, 1, 0]).T,
    np.array([0, 1, 0, 0, 1, 1, 1]).T,
    np.array([1, 1, 1, 1, 1, 1, 1]).T]

for r in r_list:
    # Syndrome
    z = H @ r % 2

    # Solve H(r + n) = z for noise vector n by least square method
    n = np.linalg.solve(H.T @ H % 2, H.T @ z % 2) % 2
    print('r         =', r)
    print('n         =', n.astype(int))
    print('decoded r =', (r - n % 2).astype(int), '\n')
