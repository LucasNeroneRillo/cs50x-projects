#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

string cipher(string str, int k);

int main(int argc, string argv[])
{
    // Check if user put exactly two arguments
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    
    // Check if key given as the second argument is a number
    for (int i = 0, n = strlen(argv[1]); i < n; i++)
    {
        if (isdigit(argv[1][i]) == false)
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }
    
    // Convert user's key from string to int
    int key = atoi(argv[1]);
    
    // Prompt user for plaintext
    string plaintext = get_string("plaintext: ");
    
    // Cipher the text according to key
    string ciphertext = cipher(plaintext, key);
    
    // Print the result
    printf("ciphertext: %s\n", ciphertext);
}

string cipher(string str, int k)
{
    // Cipher "str" using the key "k"
    for (int i = 0, n = strlen(str); i < n; i++)
    {
        // Cipher current char if it is a letter
        if (isalpha(str[i]))
        {
            // Get position of current letter in alphabet (A/a is 0, B/b is 1, and so forth)
            int position = tolower(str[i]) - 'a';
            
            // Update position according to key
            position = (position + k) % 26;
            
            // Update current letter
            if (isupper(str[i]))
            {
                str[i] = position + 'A';
            }
            else
            {
                str[i] = position + 'a';
            }
        }
    }
    
    return str;
}