
#include "utils.h"
#include <string.h>
/*
Finds the associated index of the field, for the attribute provided
*/
int find_attribute_index(Table* table, char* attribute) { // assumes its in
    for (int i = 0; i < table->fields->size; i++) {
        if (!strcmp(attribute, table->fields->array[i])) {
            return i;
        }
    }
    return -1;
}

/*
Checks whether an integer value is present in an arraylist of integers
*/
bool value_in_arr(ArrayList* arr, int val) {
    for (int i = 0; i < arr->size; i++) {
        if (*((int *)arr->array[i]) == val) {
            return true;
        }
    }
    return false;
}

/*
Frees records which contains arraylists inside the main arraylists
Calls the free_array_list function on each one of these araylsits
*/
void free_records(ArrayList* array, ArrayList* inds) {
    for (int i = 0; i < array->size; i++) {
        if (inds == NULL || inds->size == 0) {
            free_array_list(array->array[i]);
            continue;
        } 
        ArrayList* record = array->array[i];
        for (int j = 0; j < record->size; j++) {
            if (value_in_arr(inds, j)) {
                free(record->array[j]);
            } else {
                ArrayList* element = record->array[j];
                free_array_list(element);
            }
        }
        free(record->array);
        free(record);
    }
    free(array->array);
    free(array);

}


/*
Adds a copy of each element from source arraylist into destination arraylist
*/
void transfer_elements(ArrayList* dest, ArrayList* src) {
    for (int i = 0; i < src->size; i++) {
        add_to_arraylist(dest, strdup((char*) src->array[i]));
    }
}

/*
Makes a copy an arraylist and makes a copy of the all the strings that are held in the array
*/
ArrayList* make_array_copy(ArrayList* original) {
    ArrayList* copy = initialize_arraylist();
    transfer_elements(copy, original);
    return copy; 
}



/*
Makes a copy of records which stores arraylists inside the original arraylist
It calls make_array_copy for each arraylist inside of it
*/
ArrayList* make_record_copy(ArrayList* original) {
    ArrayList* copy = initialize_arraylist();
    for (int i = 0; i < original->size; i++) {
        add_to_arraylist(copy, make_array_copy(original->array[i]));
    }
    return copy;

}
