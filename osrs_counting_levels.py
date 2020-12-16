import numpy as np


def experience(level):
    return np.sum([np.floor(0.25 * (L - 1 + 300 * np.floor(2**((L - 1) / 7.)))) for L in range(2, level + 1)])


def experience_diff(level1, level2):
    return experience(level1) - experience(level2)


level1 = 70
level2 = 63
experience_rate = 17e3  # [exp / h]
print(f'experience between levels {level2} and {level1} is {int(experience_diff(level1, level2))}')
print(f'with {experience_rate} exp/h, it will take {int(experience_diff(level1, level2) / experience_rate)} h of grinding')
