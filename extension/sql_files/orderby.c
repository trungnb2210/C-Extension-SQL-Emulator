#include "orderby.h"
#include <ctype.h>
#define FLIP_IF_DESCENDING (ascending ? 1 : -1)


/*
Comparator that compares two rows based on the lexicographicl ordering 
produced by the oeprands into the ORDER BY (ie. if we had A, B, C then the
ordering would be based on that)
Different cases have to be considered on whether the items are strings or ints
and the value has to be flipped dependent on whether it is ascending or
descending
*/
int record_comparator(ArrayList* first_row, ArrayList* second_row, int *target_indices, int num_targets) {
    int cmp;
    for (int i = 0; i < num_targets; i++) {
        bool ascending = target_indices[i] > 0;
        char *first_row_target = first_row->array[abs(target_indices[i]) - 1];
        char *second_row_target = second_row->array[abs(target_indices[i]) - 1];
        if (isdigit(first_row_target[0])) {
            cmp = (atoi(first_row_target) - atoi(second_row_target));
        } else {
            cmp = strcmp(first_row_target, second_row_target);
        }
        cmp *= FLIP_IF_DESCENDING;
        if (cmp != 0) 
            return cmp;
    }
    return 0; 
}
/*
Bubble sort is used as a third argument is needed (target_indices) which isn't
supported by qsort without the use of global variables and qsort_r its 
alternative which contains this functionality is non-portable
*/
void bubbleSort(void **arr, int size, int *target_indices, int num_targets) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (record_comparator(arr[j], arr[j+1], target_indices, num_targets) > 0) {
                ArrayList* temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

/*
Parses the given operands considering whether ASC or DESC is needed, 
with ascending being the default if neither are present
Then the associated index is found for the fields which are needed to be compared
and this information is passed into the bubbleSort functions which sorts
the records in place
*/
Table* parse_order(Table* result, ArrayList* operands) {
    if (operands == NULL)
        return result;
    bool asc = !strcmp(operands->array[operands->size - 1], "ASC");
    bool desc = !strcmp(operands->array[operands->size - 1], "DESC");
    bool specified_order = (asc || desc);
    bool ascending = !desc; 
    int num_targets = operands->size - 1 - (specified_order);
    int target_attributes[num_targets];
    int curr_pos = 0;
    for (int i = 1; i < operands->size - (specified_order); i++) {
        target_attributes[curr_pos++] = (find_attribute_index(result, operands->array[i]) + 1) * FLIP_IF_DESCENDING;
    }
    bubbleSort(result->records->array, result->records->size, target_attributes, num_targets);
    return result;
}
