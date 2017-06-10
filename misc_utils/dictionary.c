
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "dictionary.h"
#include "debug_util.h"

#define     INITIAL_SIZE        4
#define     GROWTH_FACTOR       2
#define     MAX_LOAD_FACTOR     1
#define     MULTIPLIER          97

// Code inspired by
// http://www.cs.yale.edu/homes/aspnes/pinewiki/C(2f)HashTables.html?highlight=%28CategoryAlgorithmNotes%29

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

// Initialize the dictionary
dictionary internal_dict_create(int size) {
    
    dictionary dict;

    dict = malloc(sizeof(*dict));
    if (dict == 0)
        return NULL;

    dict->size = size;
    dict->count = 0;

    dict->table = malloc(sizeof(struct entry *) * dict->size);
    if (dict->table == 0) {
        free(dict);
        return NULL;
    }

    for(int i = 0; i < dict->size; i++)
        dict->table[i] = 0;

    return dict;
}

// Create a dictionary
dictionary create_dict() {
    return internal_dict_create(INITIAL_SIZE);
}

// Destroy a dictionary
void destroy_dict(dictionary dict) {

    struct entry *item;
    struct entry *next;

    for (int i = 0; i < dict->size; i++) {
        for (item = dict->table[i]; item != 0; item = next) {
            next = item->next;
            free(item->key);
            free(item);
        }
    }

    free(dict->table);
    free(dict);
}

// Hash function, takes a string and returns an unsigned long number
static unsigned long hash_function(const char *s) {

    unsigned const char *us;
    unsigned long h;

    h = 0;

    for (us = (unsigned const char *)s; *us; us++)
        h = h * MULTIPLIER + *us;

    return h;
}

// Grow the dictionary if it gets to large
static void grow_dict(dictionary dict) {

    dictionary d2;
    struct hash_table swap;
    struct entry *item;

    d2 = internal_dict_create(dict->size * GROWTH_FACTOR);

    for(int i = 0; i < dict->size; i++) {
        for(item = dict->table[i]; item != 0; item = item->next) {
            insert_dict(d2, item->key, item->value);
        }
    }

    swap = *dict;
    *dict = *d2;
    *d2 = swap;

    destroy_dict(d2);
}

// Insert key and value pair into the dictionary
void insert_dict(dictionary dict, const char *key, int32_t value) {

    struct entry *item;
    unsigned long h;

    if (key == NULL)
        return;

    item = malloc(sizeof(*item));

    if (item == NULL)
        return;

    item->key = strdup(key);
    item->value = value;

    h = hash_function(key) % dict->size;

    item->next = dict->table[h];
    dict->table[h] = item;

    dict->count++;
    if (dict->count >= dict->size * MAX_LOAD_FACTOR)
        grow_dict(dict);
}

// Search the dictionary for a key and return the value
int32_t search_dict(dictionary dict, const char *key) {

    struct entry *item;

    for (item = dict->table[hash_function(key) % dict->size];
         item != NULL;
         item = item->next) {

        if (!strcmp(item->key, key))
            return item->value;
    }

    return 0;
}

// Delete an item from the dictionary
void delete_dict(dictionary dict, const char *key) {

    struct entry **prev;
    struct entry *item;

    for( prev = &(dict->table[hash_function(key) % dict->size]);
         *prev != NULL;
         prev = &((*prev)->next)  )  {

        if (!strcmp((*prev)->key, key)) {
            item = *prev;
            *prev = item->next;

            free(item->key);
            free(item);

            return;
        }
    }
}

void print_dict(dictionary dict, FILE *out_file, const char *name) {
    
    int i;
    struct entry *item;

    debug_print("@bw", out_file, "\n\n------------------------------------------");
    debug_print("@bw", out_file, "\n       DICTIONARY  (%s)\n", name);
    debug_print("@bw", out_file, "------------------------------------------\n\n");
    
    for (i = 0; i < dict->size; i++){
        debug_print("@bw", out_file, "[%5d]\n", i);
        item = dict->table[i];
        while (item != NULL) {
            debug_print("@bw", out_file, "[-----]> (%5d) %s\n",
                item->value, item->key);
            item = item->next;
        }
    }
}
