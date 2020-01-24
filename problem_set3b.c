#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j. this adjacency matrix for final graph
bool locked[MAX][MAX];

//locked_tc[i][j] is the transitive closure of locked. locked_tc[i][j] means that there is path from i to j.
bool locked_tc[MAX][MAX];

// pair.winner=i and pair.loser=j means candidate i is winning against candidate j
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];  //array containing candidate names
pair pairs[MAX * (MAX - 1) / 2];  //array pairs containing variables of type pair.

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void update_locked_tc(int winner, int loser);
void print_winner(void);

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
            locked_tc[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference. ranks[i] = j means ith preference is candidate with ID j.
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
    //if name is in candidates then add id of candidate to ranks, otherwise end election
    //we are not checking whether a voter adds each candidate exactly once
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i; //i is the candidate's ID.
            return true;
        }
    }
    return false;
}


// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    //the voter's ith favourite candidate gets +1 preference over jth favourite candidate for i<j.
    for (int i = 0; i < candidate_count - 1; i++)
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
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }

    return;
}


// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    //bubble sort with smallest bubbling down the array
    for (int i = 0; i < pair_count - 1; i++)
    {
        for (int j = 0 ; j < pair_count - 1 - i; j++)
        {
            if (preferences[pairs[j].winner][pairs[j].loser] < preferences[pairs[j + 1].winner][pairs[j + 1].loser])
            {
                pair temp;
                temp.winner = pairs[j].winner;
                temp.loser = pairs[j].loser;
                pairs[j].winner = pairs[j + 1].winner;
                pairs[j].loser = pairs[j + 1].loser;
                pairs[j + 1].winner = temp.winner;
                pairs[j + 1].loser = temp.loser;
            }
        }
    }

    //checks checks checks
    /*for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            printf("%i", preferences[i][j]);
        }
        printf("\n");
    }

    for (int k = 0; k < pair_count; k++)
    {
        printf("The %ith pair is %i-%i-%i\n", k, pairs[k].winner, pairs[k].loser, preferences[pairs[k].winner][pairs[k].loser]);
    }*/
    return;
}


// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    int winner;
    int loser;

    for (int i = 0; i < pair_count; i++)
    {
        winner = pairs[i].winner;
        loser = pairs[i].loser;

        //check if adding new edge winner->loser will create cycle
        //cycle will be created iff there is already a path from loser to winner
        if (!(locked_tc[loser][winner]))
        {
            //add edge to locked and locked_tc
            locked[winner][loser] = true;
            locked_tc[winner][loser] = true;

            //update locked_tc
            update_locked_tc(winner, loser);
        }
    }

    //checkscheckschecks
    /*printf("new locked is: \n");
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            printf("%i",locked[i][j]);
        }
        printf("\n");
    }*/

    return;
}


//updated locked_tc
void update_locked_tc(int winner, int loser)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            //if there is path from i to winner, there is now a path from i to loser
            if (locked_tc[i][winner])
            {
                locked_tc[i][loser] = true;
            }

            //if there is path from loser to j, there is now a path from winner to j
            if (locked_tc[loser][j])
            {
                locked_tc[winner][j] = true;
            }

            //if there is path from i to winner and from loser to j, there is now a path from i to j
            if (locked_tc[i][winner] && locked_tc[loser][j])
            {
                locked_tc[i][j] = true;
            }
        }
    }

    //checkscheckschecks
    /*printf("new locked_tc is: \n");
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            printf("%i",locked_tc[i][j]);
        }
        printf("\n");
    }*/


    return;
}


// Print the winner of the election
void print_winner(void)
{
    //for each i, find out if there is any j such that j beats i
    //if no such j exists, i is a source so print i as winner
    for (int i = 0; i < candidate_count; i++)
    {
        bool source = true; //this variable tracks whether i is a source or not

        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i])
            {
                source = false;
            }
        }

        if (source)
        {
            printf("%s\n", candidates[i]);
            return;
        }
    }
    return;
}
