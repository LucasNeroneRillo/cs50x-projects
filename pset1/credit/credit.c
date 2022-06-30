#include <stdio.h>
#include <cs50.h>
#include <math.h>

bool luhn_algorithm(long card, int nmbr_of_dgts);
char card_brand(long card, int nmbr_of_dgts);

int main(void)
{
    // Ask user for credit-card number (must be a non-negative value)
    long credit_card = -1;
    while (credit_card < 0)
    {
        credit_card = get_long("Number: ");
    }
    
    // Get number of digits in credit-card number
    int digits = 0;
    while (pow(10, digits) <= credit_card)
    {
        digits++;
    }
    
    // Run Luhn's Algorithm
    // Function returns true if card is valid per Luhn's Algorithm (returns false otherwise)
    bool luhn = luhn_algorithm(credit_card, digits);

    //Check if VISA, MASTERCARD, AMEX, or INVALID
    char result = card_brand(credit_card, digits);

    // Print result
    if (luhn == false || result == 'i')
    {
        printf("INVALID\n");
    }
    else if (result == 'a')
    {
        printf("AMEX\n");
    }
    else if (result == 'v')
    {
        printf("VISA\n");
    }
    else
    {
        printf("MASTERCARD\n");
    }
}

bool luhn_algorithm(long card, int nmbr_of_dgts)
{
    // Innitialize the sum and start loop (one iteration for each of card's digits)
    int sum = 0;
    for (int i = 0; i < nmbr_of_dgts; i++)
    {
        // Save last digit of card in current_digit
        int current_digit = card % 10;

        // Remove last digit from card
        card -= current_digit;
        card /= 10;

        // Add current_digit to sum (multiplied by two if needed)
        if (i % 2 == 0)
        {
            sum += current_digit;
        }
        else
        {
            int n = 2 * current_digit;

            // If n has two digits, add them (n's first digit is always 1 in that case)
            if (n > 9)
            {
                n = (n % 10) + 1;
            }

            sum += n;
        }
    }

    // Check result of luhn's algorithm
    if (sum % 10 == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

char card_brand(long card, int nmbr_of_dgts)
{
    // Check if AMEX
    if (nmbr_of_dgts == 15)
    {
        int first_digits = card / pow(10, nmbr_of_dgts - 2);
        if (first_digits == 34 || first_digits == 37)
        {
            return 'a';
        }
    }

    // Check if MASTERCARD
    if (nmbr_of_dgts == 16)
    {
        int first_digits = card / pow(10, nmbr_of_dgts - 2);
        if (first_digits > 50 && first_digits < 56)
        {
            return 'm';
        }
    }

    // Check if VISA
    if (nmbr_of_dgts == 16 || nmbr_of_dgts == 13)
    {
        int first_digit = card / pow(10, nmbr_of_dgts - 1);
        if (first_digit == 4)
        {
            return 'v';
        }
    }

    // If function has not returned yet, card is invalid
    return 'i';
}