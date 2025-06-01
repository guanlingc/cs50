// Implements a dictionary's functionality

#include "dictionary.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
// 2^16
const unsigned int N = 65536;
int word_count = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    int hash_value = hash(word);
    node *cursor = table[hash_value];
    while (cursor != NULL)
    {
        if (strcasecmp(word, cursor->word) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    return toupper(word[0]) - 'A';
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    // allocate memory for hashtable
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        fprintf(stderr, "There has been an error\n");
        return false;
    }
    // declare variable to store word from dict
    char temp_word_store[LENGTH + 1]; // length comes from dictionary.h plus 1 for null operator
    // loop the words in variable[file] until end-of-file
    while (fscanf(file, "%s", temp_word_store) != EOF)
    {
        // declare memory for new_node and check if allocation is a success
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            return 1;
        }
        // if checks pass, copy word into node
        strcpy(new_node->word, temp_word_store);
        // hash function to get hash value
        int bucket_index = hash(temp_word_store);
        //
        if (table[bucket_index] == NULL)
        {
            new_node->next = NULL;
        }
        else
        {
            new_node->next = table[bucket_index];
        }
        // place the head to point at new node
        table[bucket_index] = new_node;
        // table
        word_count += 1;
    }
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return word_count;
}

void free_memory(node *n)
{
    if (n->next != NULL)
    {
        free_memory(n->next);
    }
    free(n);
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for (int i = 0; i < 26; i++)
    {
        if (table[i] != NULL)
        {
            free_memory(table[i]);
        }
    }

    return true;
}
