#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    // Prompt user for change owed, in dollars
    float dollars = -1;
    while (dollars < 0)
    {
        dollars = get_float("Change oewd: ");
    }

    // Convert the ammount owed to cents (rounded to nearest penny)
    int cents = round(100 * dollars);

    // Innitialize variable "coins"  to count number of coins needed
    int coins = 0;

    // Calculate number of quarters needed, subtracting the ammount paid with quarters from total owed
    int quarters = cents / 25;
    coins += quarters;
    cents -= quarters * 25;

    // Repeat the process for dimes, nickels, and pennys
    int dimes = cents / 10;
    coins += dimes;
    cents -= dimes * 10;

    int nickels = cents / 5;
    coins += nickels;
    cents -= nickels * 5;

    int pennys = cents;
    coins += pennys;
    cents -= pennys;

    // Print number of coins needed
    printf("%i\n", coins);
}