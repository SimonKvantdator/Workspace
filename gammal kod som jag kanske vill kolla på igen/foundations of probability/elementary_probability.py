import numpy as np
from scipy.special import binom

### 1.1 - probability of events ###

# Q1
print(0.1 + 0.44)
print(0.1 + 0.02)
print()

# Q2
print(1.0 - 0.09 - 0.11)
print(1.0 - 0.09)
print()

# Q3
# for each outcome of the 8-sided die, there is one outcome of the 12-sided die that gives a total score of 10, each such outcome has probability 1/12
print(1 * 1.0 / 12)
print()

# Q4
print(1.0 / 4)
print(0)  # Numeric intersection Face = empty set
print(9.0 / 13)  # Numeric union Odd = Numeric
print(1.0 - 1.0 / 4)  # not Diamond
print(4.0 / 13)  # Odd subset of not Ace
print(1.0 / 2)  # Heart subset of not Black
print((1.0 / 2) * (8.0 / 13))  # events Black and not Even are independet
print(1.0)  # Ace intersection Odd = empty set
print()

# Q5
print(13.0 / 51)  # (nbr of diamond) / (nbr of cards)
print(5.0 / 51)  # (nbr of even spades) / (nbr of cards)
print(2.0 * 13.0 / 51)  # Diamond subset of Red, so (nbr of reds) / (nbr cards)
print(2.0 * 13 / 51)
print()

# Q6
print(binom(52, 5))
print(binom(52 - 5, 5) / binom(52, 5))  # Delen / det hela
print((5 / 52) * (binom(51 - 4, 4) / binom(51, 4)) * 5)



