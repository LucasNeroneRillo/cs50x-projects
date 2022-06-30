// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <math.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table (was 17575)
const unsigned int N = 17575;

// Number of words in dictionary
int nmbr_of_words = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Hash the word (hash function takes lowercase word as input)
    int length = strlen(word);
    char *lower_word = malloc(length + 1);
    for (int i = 0; i <= length; i++)
    {
        lower_word[i] = tolower(word[i]);
    }
    int index = hash(lower_word);
    free(lower_word);

    // Look for word in hash table
    for (node *tmp = table[index]; tmp != NULL; tmp = tmp->next)
    {
        // Case insensitevely compare the word in linked-list with the one asked
        if (strcasecmp(tmp->word, word) == 0)
        {
            return true;
        }
    }

    // Word was not found
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    int index = 0;

    for (int i = 0, n = strlen(word); i < n; i++)
    {
        index += i * i * word[i];
    }

    index = index % N;
    return index;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open dictionary
    FILE *file = fopen(dictionary, "r");

    // Return false if dictionary not opened
    if (file == NULL)
    {
        return false;
    }

    // Read all words from file, one at a time
    char word[LENGTH + 1];
    while (fscanf(file, "%s", word) != EOF)
    {
        // Create a new node
        node *n = malloc(sizeof(node));

        // Return false if memory was not allocated
        if (n == NULL)
        {
            return false;
        }

        // Copy the word read into the node created
        strcpy(n->word, word);

        // Hash the word
        int index = hash(word);

        // Insert the node into the hash table
        n->next = table[index];
        table[index] = n;

        // Update number of words in dictionary
        nmbr_of_words++;
    }

    fclose(file);

    // Success
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return nmbr_of_words;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // Each i iteration frees one linked list of the hash table's array
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        while (cursor != NULL)
        {
            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
    }

    return true;
}
