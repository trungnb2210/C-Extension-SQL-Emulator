#include "groupby.h"
#include "table.h"
#include <stdlib.h>
#include <stdbool.h>

/*
Initialises a new record for the output from an existing record from the input
that contains the same data except items in columns that are not mentioned in the 
GROUP BY line as operands are now stored in an nested ArrayList
*/
static ArrayList* initialise_record(ArrayList* record, ArrayList* inds, Table* result) {
    ArrayList* new_arr_list = initialize_arraylist();
    for (int x = 0; x < result->fields->size; x++) {
        if (value_in_arr(inds, x)) {
            add_to_arraylist(new_arr_list, strdup(record->array[x]));
        } else {
            ArrayList* element_list = initialize_arraylist();
            add_to_arraylist(element_list, strdup(record->array[x]));
            add_to_arraylist(new_arr_list, element_list);
        }
    }
    return new_arr_list;
}

/*
Takes in two records and checks if the values of the elements in the columns listed in
the GROUP BY operands arraylist are exactly the same in both records. If they are, 
the function returns true, otherwise it returns false.
*/
static bool records_match(ArrayList* record, ArrayList* output_record, ArrayList* operands, ArrayList* inds) {
    for (int k = 0; k < operands->size - 1; k++) {
        int index = *((int*) inds->array[k]);
        if (strcmp(record->array[index], output_record->array[index])) {
            return false;
        }
    }
    return true;
}

/* 
From the operands, determines the indices of the fields array of result that are
columns listed in the GROUP BY operands arraylist and stores these indices in the
arraylist inds. It also determines the indices of result's fields array that are 
columns NOT listed in the GROUP BY operands arraylist and stores these indices in
the array non_inds.
*/
static void calculate_used_indices(Table* result, ArrayList* operands, Table* output, ArrayList* inds, int* non_inds) {
    int non_inds_size = 0;
    for (int i = 0; i < result->fields->size; i++) {
        add_to_arraylist(output->fields, strdup(result->fields->array[i]));
        bool added = false;
        for (int j = 1; j < operands->size; j++) {
            if (!strcmp(result->fields->array[i], operands->array[j])) {
                int* ind = malloc(sizeof(int));
                *ind = i;
                add_to_arraylist(inds, ind);
                added = true;
                break;
            }
        }
        if (!added) 
            non_inds[non_inds_size++] = i;
    }
}

/*
Loops through each existing record and for each record, uses records_match to 
check if there already exists a record in output that has the same values for 
the columns mentioned in the GROUP BY operands. If there is, then the values of 
the record in columns that are not in the GROUP BY operands are added to their
respective ArrayLists in the matched existing record. If there is not, we
initialise and add a new record in output specialised for this GROUP BY by 
calling the initialise_record function.
*/

static void create_all_records(Table* result, ArrayList* operands, Table* output, ArrayList* inds, int* non_inds) {
    for (int i = 0; i < result->records->size; i++) {
        ArrayList* record = result->records->array[i];
        bool record_is_intialised = false;
        for (int j = 0; j < output->records->size; j++) {
            ArrayList* output_record = output->records->array[j];
            if (records_match(record, output_record, operands, inds)) {
                for (int k = 0; k < (result->fields->size) - (operands->size - 1); k++) {
                    add_to_arraylist(output_record->array[non_inds[k]], strdup(record->array[non_inds[k]]));
                }
                record_is_intialised = true;
            }
        }
        if (!record_is_intialised) 
            add_to_arraylist(output->records, initialise_record(record, inds, result));
    }
}

/*
If there are no GROUP BY operands, then the output table is the same as the
input table. If there are, we create a new table, store the field indices
for columns in the GROUP BY operands and not in the GROUP BY operands by calling
calculate_used_indices. We then create and store all the required records needed
for the GROUP BY by calling the create_all_records function. In each record, 
the values of elements in the GROUP BY operands are stored as char * and the
values of elements not in the GROUP BY operands are stored in an ArrayList. 
There will only be one element for each unique combination element values for 
the columns in the GROUP BY operands.
*/
Table* parse_group(Table* result, ArrayList* operands, ArrayList* inds) {
    if (operands == NULL)
        return result;

    Table* output = init_table();
    int non_inds[result->fields->size];

    calculate_used_indices(result, operands, output, inds, non_inds);
    create_all_records(result, operands, output, inds, non_inds);

    free_table(result, NO_INDICES);
    return output;
}
