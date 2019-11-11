// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents number of buckets in a hash table
#define N 26

char temp_string[LENGTH + 1];

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Represents a hash table
node *hashtable[N];

// Hashes word to a number between 0 and 25, inclusive, based on its first letter
unsigned int hash(const char *word)
{
    return tolower(word[0]) - 'a';
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize hash table
    for (int i = 0; i < N; i++)
    {
        hashtable[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    // Insert words into hash table
    while (fscanf(file, "%s", word) != EOF)
    {
        // TODO
        int hash_index = hash(word);

        // creating a node:
        node *n = malloc(sizeof(node));
        strcpy(n->word, word);
        n->next = NULL;

//        if(hashtable[hash_idex] = NULL)
//        {
//            hashtable[hash_index] = n;
//        }
//        else
//        {
//            n->next = hashtable[hash_index];
//            hashtable[hash_index] = n;
//        }

        n->next = hashtable[hash_index];
        hashtable[hash_index] = n;
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    unsigned int number_of_words = 0;
    for (int i = 0; i < 26; i++)
    {
        node *temp;
        temp = hashtable[i];

        if (temp == NULL)
        {
            continue;
        }
        else
        {
            while (temp != NULL)
            {
                number_of_words++;
                temp = temp->next;
            }
        }
    }

    return number_of_words;
}

// Returns true if word is in dictionary else false

bool compare(char word1[LENGTH + 1], const char word2[LENGTH + 1])
{
    for (int i = 0; word1[i] != '\0' || word2[i] != '\0'; i++)
    {
        if (tolower(word1[i]) != tolower(word2[i]))
        {
            return false;
        }
    }
    return true;
}

bool check(const char *word)
{
    // TODO
    unsigned int hash_index;
    hash_index = hash(&word[0]);

    node *temp;
    temp = hashtable[hash_index];
    while (temp != NULL)
    {
        if (compare(temp->word, word))
        {
            return true;
        }
        temp = temp->next;
    }

    return false;
}

//function that deallocates a linked list
void deallocatelist(node *head)
{
    node *tmp;

    while (head != NULL)
    {
        tmp = head;
        head = head->next;
        free(tmp);
    }

}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < 26; i++)
    {
        deallocatelist(hashtable[i]);
    }
    return true;
}
