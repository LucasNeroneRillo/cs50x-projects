from cs50 import get_int
import sys

# Ask user for credit-card number
credit_card = -1
while(credit_card < 0):
    credit_card = get_int("Number: ")

# Transform credit card into string
card = str(credit_card)

# Get number of digits in credit-card number
digits = len(card)

# Luhn algortithm
sum = 0
for i in range(digits):

    # Add digit to sum (multiplied by two if needed)
    # Start adding last digit and end adding first
    if i % 2 == 0:
        sum += int(card[digits - i - 1])
    else:
        # If n has two digits, add them
        n = 2 * int(card[digits - i - 1])
        if n > 9:
            n = (n % 10) + 1
        sum += n

# Check if, per algortrithm, credit is invalid
if sum % 10 != 0:
    print("INVALID")
    sys.exit(1)

# Check if AMEX
if digits == 15 and card[0] == "3" and card[1] in ["4", "7"]:
    print("AMEX")
    sys.exit(0)

# Check if MASTERCARD
elif digits == 16 and card[0] == "5" and int(card[1]) in range(6):
    print("MASTERCARD")
    sys.exit(0)

# Check if VISA
elif digits in [13, 16] and card[0] == "4":
    print("VISA")
    sys.exit(0)

# Card must be invalid
else:
    print("INVALID")
    sys.ecit(1)