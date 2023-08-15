#ifndef ARRAYLIST_H
#define ARRAYLIST_H
#include <stdlib.h>
#include <stdbool.h>

/*
    Capacity refers to maximum number of int elements allowed in array. Size refers to
    current number of int elements in array. Capacity and size allows us to reallocate
    memory as needed when adding more elements to array.
*/
typedef struct {
    void** array;
    int capacity;
    int size;
} ArrayList;

bool value_in_arr(ArrayList *arr, int val);
extern ArrayList* initialize_arraylist(void);
extern void add_to_arraylist(ArrayList*, void*);
extern void free_array_list(ArrayList*);

#endif