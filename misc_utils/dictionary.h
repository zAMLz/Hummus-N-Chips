
#ifndef __DICTIONARY_H__
#define __DICTIONARY_H__

#include <stdint.h>
#include <stdio.h>

// Entry stuct. Supports hash chaining
struct entry {
    struct entry *next;
    char *key;
    int32_t value;
};

// Dictionary structure
struct hash_table {
    int size;
    int count;
    struct entry **table;
};

// define a typedef for the dictionary structure
typedef struct hash_table *dictionary;


// Create an empty dictionary
dictionary create_dict(void);

// Destroy the dictionary
void destroy_dict(dictionary dict);

// Inser a key-value pair into the dictionary
void insert_dict (dictionary dict, const char *key, int32_t value);

// Return the value associated with a given key or return 0 if none found
int32_t search_dict(dictionary dict, const char *key);

// Delete a key-value pair from the dictionary from a given key
void delete_dict(dictionary dict, const char *key);

#endif