#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // TODO: Prompt for start size
    int start = -1;
    while (start < 9)
    {
        start = get_int("Start size: ");
    }

    // TODO: Prompt for end size
    int end = -1;
    while (end < start)
    {
        end = get_int("End size: ");
    }

    // TODO: Calculate number of years until we reach threshold
    int years = 0;
    int population = start;
    while (population < end)
    {
        population += (population / 3) - (population / 4);
        years++;
    }

    // TODO: Print number of years
    printf("Years: %i\n", years);
}