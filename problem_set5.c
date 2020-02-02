// Implements a dictionary's functionality

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"



// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 100000;

// Hash table.
node *table[N];



// Returns true if word is in dictionary else false
bool check(const char *word)
{
    //create lowercase copy of word
    char word_lower[LENGTH + 1];

    for (int i = 0, l = strlen(word); i <= l; i++)
    {
        word_lower[i] = tolower(word[i]);
    }

    //initialise temporary pointer.
    node *temp = table[hash(word_lower)];

    //go through linked list and search for word
    while (temp != NULL)
    {
        if (strcmp(temp -> word, word_lower) == 0)
        {
            return true;
        }
        else
        {
            temp = temp -> next;
        }
    }
    return false;
}



// Hashes word to a number
//https://stackoverflow.com/questions/2624192/good-hash-function-for-strings
unsigned int hash(const char *word)
{
    int hash = 7;
    for (int i = 0, l = strlen(word); i < l; i++)
    {
        hash = (hash * 31 + word[i]);
    }

    return hash % N;
}



// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    //open dictionary file
    FILE *fp_dictionary = fopen(dictionary, "r");
    if (fp_dictionary == NULL)
    {
        printf("Failed to fopen %s\n", dictionary);
        return false;
    }

    //intialise variables
    char word[LENGTH + 1];
    int index = 0;

    //ensure hash table starts with null pointers
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }


    //go through dictionary file adding words to hashtable
    for (int c = fgetc(fp_dictionary); c != EOF; c = fgetc(fp_dictionary))
    {
        //if we have newline character then we are at end of word. add word to hashtable
        if (c == 10)
        {
            word[index] = '\0';
            index = 0;

            bool loaded = load_word(word);
            if (!loaded)
            {
                printf("Could not load %s.\n", word);
                return false;
            }
        }

        //if no newline character, add character to end of word
        else
        {
            word[index] = c;
            index++;
        }
    }

    //close file
    fclose(fp_dictionary);

    return true;
}



// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    //initialise variables
    int counter = 0;
    node *temp = NULL;

    //loop through hash table, incrementing counter by 1 for each non-null pointer
    for (int i = 0; i < N; i++)
    {
        temp = table[i];

        while (temp != NULL)
        {
            counter++;
            temp = temp -> next;
        }
    }

    return counter;
}



// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    //initialise variables. by construction, temp_prev -> next = temp
    node *temp = NULL;
    node *temp_prev = NULL;

    //loop through table, freeing all the mallocs
    for (int i = 0; i < N; i++)
    {
        temp = table[i];
        while (temp != NULL)
        {
            temp_prev = temp;
            temp = temp -> next;
            free(temp_prev);
        }
    }

    return true;
}


//adds word to hash table
bool load_word(const char *word)
{
    //initalise temporary pointer
    node *temp = table[hash(word)];

    //need sepearate case for creating first item in the list
    if (temp == NULL)
    {
        //allocate new block of memory
        temp = malloc(sizeof(node));

        //check for null
        if (temp == NULL)
        {
            printf("Malloc for new node failed\n");
            return false;
        }

        //fill node with data
        temp -> next = NULL;
        strcpy(temp -> word, word);

        //update pointer in table
        table[hash(word)] = temp;

        return true;
    }

    //get to end of linked list
    while (temp -> next != NULL)
    {
        temp = temp -> next;
    }

    //allocate new block of memory.
    temp -> next = malloc(sizeof(node));
    temp = temp -> next;

    //check for null
    if (temp == NULL)
    {
        printf("Malloc for new node failed\n");
        return false;
    }

    //fill node with data
    temp -> next = NULL;
    strcpy(temp -> word, word);

    return true;
}
