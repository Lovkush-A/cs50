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
const unsigned int N = 10000;

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
        hash = (hash*31 + word[i]) % N;
    }
    return hash;
}



// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    //open dictionary file
    FILE* fp_dictionary = fopen(dictionary, "r");
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

        //prematurely stop loading
        //if (strcmp(word, "able") == 0)
        //{
        //    return true;
        //}
    }

    return true;
}



// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return 0;
}



// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO
    return false;
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



void test(void)
{

}