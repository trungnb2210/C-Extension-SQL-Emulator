#include "arraylist.h"
#include <string.h>
#include <stdio.h>

/*
Dynamically allocates the required memory for the arraylist and returns
this new dynamically allocated arraylist
*/
ArrayList* initialize_arraylist(void) {
    ArrayList *array = malloc(sizeof(ArrayList));
    array->capacity = 1;
    array->size = 0;
    array->array = malloc(array->capacity * sizeof(void *));
    return array;
}

/*
Helper function used by add_to_arraylist which doubles the size of the arraylist
if the capcity is reached
*/
static void resize_array(ArrayList* array) {
    if (array->size == array->capacity) {
        array->capacity *= 2;
        array->array = realloc(array->array, array->capacity * sizeof(void *));
    }
}

/*
Adds an item (void *) so of any type to to an arraylist
Calls resize array incase maximum capacity is reached and it needs to grow
*/
void add_to_arraylist(ArrayList* array, void* item) {
    resize_array(array);
    array->array[array->size++] = item;
}

/*
Frees all items inside the arraylist and the pointer to the array and the arraylist itself
*/
void free_array_list(ArrayList* array) {
    for (int i = 0; i < array->size; i++) {
        free(array->array[i]);
    }
    free(array->array);
    free(array);
}


