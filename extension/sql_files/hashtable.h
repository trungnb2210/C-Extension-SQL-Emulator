#ifndef HASHTABLE_H
#define HASHTABLE_H
 
/*
    Each entry of the hashtable stores the key, value, and a pointer to the next entry (in the linked list) 
    as its fields. The value is stored as a void pointer so we can have generic types, so HashTable
    can be used for different purposes.
*/
typedef struct Entry {
    char* key;
    void* value;
    struct Entry* next;
} Entry;

typedef Entry** HashTable;

extern HashTable initialise_hash_table(void);
extern void insert(HashTable, const char*, void*);
extern void* get(HashTable, const char*);
extern void free_hash_table(HashTable, int);

#endif