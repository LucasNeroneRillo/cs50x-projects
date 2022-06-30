from cs50 import get_float

# Ask user for change owed and convert it to cents
dollars = -1
while dollars < 0:
    dollars = get_float("Owed: ")
cents = round(100 * dollars)

# Calculate number of quarters required
# Subtract ammount paid with quarters from total owed
q = int(cents / 25)
cents -= 25 * q

# Repeat the process for dimes, nickels, and pennys
d = int(cents / 10)
cents -= 10 * d

n = int(cents / 5)
cents -= 5 * n

p = int(cents)
cents -= p

# Print number of coins needed
print(int(q + d + n + p))
