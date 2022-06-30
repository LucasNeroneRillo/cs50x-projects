#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool path(int from, int to);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            // "i" is the int associated with the candidate whose name matches "name"
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            // If i is prefered over j, record i as the winner
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;

                // Update number of pairs
                pair_count++;
            }

            // If j is prefered over i, record j as the winner
            if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;

                // Update number of pairs
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // Bubble sort
    bool swaps;
    for (int i = 0; i < pair_count - 1; i++)
    {
        // No swaps in this iteration yet
        swaps = false;

        for (int j = 0; j < pair_count - 2; j++)
        {
            // If j'th and j+1'th pair out of order, swap them
            int j_strenghth = preferences[pairs[j].winner][pairs[j].loser];
            int j_plus_1_strenghth = preferences[pairs[j + 1].winner][pairs[j + 1].loser];
            if (j_strenghth < j_plus_1_strenghth)
            {
                // Swap
                pair weakest = pairs[j];
                pairs[j] = pairs[j + 1];
                pairs[j + 1] = weakest;

                // In this iteration, swaps were made
                swaps = true;
            }
        }

        if (swaps == false)
        {
            return;
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        // If no cycle will be created, lock the pair
        if (path(pairs[i].loser, pairs[i].winner) == false)
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
}

// Print the winner of the election
void print_winner(void)
{
    //  Find candidate with no arrows pointing to them
    for (int i = 0; i < candidate_count; i++)
    {
        int pointed_at = false;

        // If someone is locking i, i is not the winner
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i] == true)
            {
                pointed_at = true;
            }
        }

        // If i is not pointed at, it is the winner
        if (pointed_at == false)
        {
            printf("Winner: %s\n", candidates[i]);
        }
    }
    return;
}

// Check if there is a path from loser to winner (returns true if there is a path)
bool path(int from, int to)
{
    // If direct path, return true (recursion's base case)
    if (locked[from][to] == true)
    {
        return true;
    }

    // For all candidates:
    for (int i = 0; i < candidate_count; i++)
    {
        // If candidate has arrow pointing to goal("to"):
        if (locked[i][to] == true)
        {
            // If there is a path from source("from") to candidate("i"), return true
            if (path(from, i) == true)
            {
                return true;
            }
        }
    }

    return false;
}

