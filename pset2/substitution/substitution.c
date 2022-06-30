#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>

int main(int argc, string argv[])
{
    // Check if user typed a single command-line argument besides the file name
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    // Save key inside a variable
    string key = argv[1];

    // Check if key provided has the correct size
    if (strlen(key) != 26)
    {
        printf("Key must contain 26 characters.");
        return 1;
    }


    // Check if key is valid
    for (int i = 0; i < 26; i++)
    {
        // Check if key has only letters
        if (isalpha(key[i]) == false)
        {
            printf("Key must contain only letters\n");
            return 1;
        }
        
        // Make key entirely uppercase
        key[i] = toupper(key[i]);

        // Check if each letter appears exactly once
        for (int j = i + 1; j < 26; j++)
        {
            if (key[i] == key[j])
            {
                printf("Each letter must appear exactly once\n");
                return 1;
            }
        }
    }

    // Ask for user's input
    string text = get_string("plaintext: ");

    // Cipher the plaintext
    for (int i = 0, n = strlen(text); i < n; i++)
    { 
        // Cipher current char if it is a letter
        if (isalpha(text[i]))
        {
            // Get position of current letter in alphabet (A/a is 0, B/b is 1, and so forth)
            int position = tolower(text[i]) - 'a';

            // Change current letter according to key
            if (isupper(text[i]))
            {
                text[i] = key[position];
            }
            else
            {
                text[i] = tolower(key[position]);
            }
        }
    }
    
    // Print the ciphered text
    printf("ciphertext: %s\n", text);
}