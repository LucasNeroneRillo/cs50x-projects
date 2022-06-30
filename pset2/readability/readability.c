#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

int main(void)
{
    // Prompt user for input
    string text = get_string("Text: ");
    
    // Get number of letters, words, and sentences in text
    int letters = 0, words = 1, sentences = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (isalpha(text[i]))
        {
            letters++;
        }
        else if (text[i] == ' ')
        {
            words++;
        }
        else if (text[i] == '!' || text[i] == '.' || text[i] == '?')
        {
            sentences++;
        }
    }
    
    // Get L ans S per Coleman-Liau index
    float L = 100.0 * letters / words;
    float S = 100.0 * sentences / words;
    
    // Calculate the index
    int index = round(0.0588 * L - 0.296 * S - 15.8);
    
    // Print the text's grade
    if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
}