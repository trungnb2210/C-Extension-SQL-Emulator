#include "hashtable.h"
#include "arraylist.h"
#include "ast.h"
#include "utils.h"
#include "table.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/*
    Any arbitrary table size would work, we set it as 10. 
*/
#define TABLE_SIZE 10
#define HASH_CONSTANT 31

/*
    Standard hashing (helper) function for a key (string) and TABLE_SIZE = 10.
*/
static unsigned int hash(const char* key) {
    unsigned int hash = 0;
    for (int i = 0; i < strlen(key); i++) {
        hash = (hash * HASH_CONSTANT) + key[i];
    }
    return hash % TABLE_SIZE;
}

/*
    Initializes an empty hashtable by returning a pointer to a dynamically allocated block of 10 (TABLE_SIZE) 
    entry pointers. All entry pointers are then initialised to NULL, as malloc means chunks are indeterminate
    to begin with. 
*/
HashTable initialise_hash_table(void) {
    HashTable table = (HashTable)calloc(TABLE_SIZE, sizeof(Entry*));
    assert(table != NULL);
    return table;
}

/*
    Inserts a pointer to a generic type value, with a corresponding string key into a given HashTable. The key value pair
    is stored as an Entry struct, with the next field (Entry*) set as NULL. A pointer to this struct is then stored in the 
    HashTable by adding it to the linked list of Entry's at the hashed index (0 to TABLE_SIZE). 
*/
void insert(HashTable table, const char* key, void* value) {
    unsigned int index = hash(key);
    Entry* entry = (Entry*)malloc(sizeof(Entry));
    assert(entry != NULL);
    entry->key = strdup(key);
    assert(entry->key != NULL); // Not malloc, but strdup could return a NULL pointer.
    entry->value = value;
    entry->next = NULL;

    /*
        If the Entry* of the HashTable at the hashed index is NULL, the linked list is initialized with the desired Entry as the first
        element. In the case of collision, i.e. there is already a non-empty Entry linked list at the index, the linked list 
        is traversed by accessing the pointer to the next Entry until the last Entry element is reached. Then the desired Entry is appended
        to the linked list by setting the next field (Entry*) of the last element to it. 
    */
    if (table[index] == NULL) {
        table[index] = entry;
    } else {
        Entry* current = table[index];
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = entry;
    }
}

/*
    Returns a pointer to the value stored at the specified key, in a given HashTable. It accesses the linked list at the hashed index
    of the HashTable, and traverses it until the key matches the Entry, or the end is reached. In the latter case, NULL is returned. 
*/
void* get(HashTable table, const char* key) {
    unsigned int index = hash(key);
    Entry* current = table[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current->value;
        }
        current = current->next;
    }
    return NULL; 
}

/*
    Abstracted function to handle freeing for all HashTable usages. Goes through all TABLE_SIZE (10) linked lists in the
    HashTable and frees the key (used strdup), value pointer, and the next Entry pointer. Temporary variable to stoee next Entry pointer is
    needed in the loop so traversal is possible even after the next field is freed. 
    If the HashTable stores tables, then it will also call the free_table to free all entries

    Then the actual HashTable itself is freed as well, as it is a malloc'ed pointer too. 
*/

void free_hash_table(HashTable table, int tables) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Entry* entry = table[i];
        while (entry != NULL) {
            Entry* next = entry->next;
            free(entry->key);
            if (tables) {
                free_table(entry->value, NULL);
            } else {
                free_variable_data(entry->value);
            }
            free(entry);
            entry = next;
        }
    }
    free(table);
}

