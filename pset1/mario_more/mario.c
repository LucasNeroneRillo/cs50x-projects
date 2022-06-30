#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // Get height from user
    int height = 0;
    while (height < 1 || height > 8)
    {
        height = get_int("Height: ");
    }

    // Print pyramid (each "i" iteration is one line)
    for (int i = 1; i <= height; i++)
    {
        // Print left-side spaces
        for (int j = (height - i); j > 0; j--)
        {
            printf(" ");
        }

        // Print left-side hashtags
        for (int j = 1; j <= i; j++)
        {
            printf("#");
        }

        // Print center spaces
        printf("  ");

        // Print right-side hashtags
        for (int j = 1; j <= i; j++)
        {
            printf("#");
        }

        // Print new line
        printf("\n");
    }
}