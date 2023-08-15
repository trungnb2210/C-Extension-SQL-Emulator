#include "select.h"
#include "aggregates.h"


/*
When a field is aggregated an arraylist will be stored in that respective record containing
the values that have been matched for the GROUP BY
This array list is reduced is reduced given the function specificed in the operands (ie. SUM or MAX)
by the function pointer returned parse_agg
*/
static void calcluate_aggregated_field(ArrayList* operands, ArrayList* record, ArrayList* new, Table* result, int operand_index, int result_index) {
    char *ptr = strdup(operands->array[operand_index]);
    char *p = strtok(ptr, "(");
    p = strtok(NULL, ")");
    if (!strcmp(result->fields->array[result_index], p)) {
        char *newp = strdup(operands->array[operand_index]);
        p = strtok(newp, "(");
        char* buf = (*parse_agg(p))(record->array[result_index]);
        free(newp);
        add_to_arraylist(new, buf);
    }
    free(ptr);

}
/*
We go through each record and add the columns that coincide with the specified
operands
If it it has been aggregated then it will be reduced by calculate_aggregate_field
*/
Table* parse_select(Table* result, ArrayList* operands, ArrayList* inds) {
    Table* output = init_table();
    transfer_elements(output->fields, operands);
    for (int i = 0; i < result->records->size; i++) {
        ArrayList *record = result->records->array[i];
        ArrayList *new = initialize_arraylist();
        for (int k = 0; k < operands->size; k++) {
             for (int j = 0; j < record->size; j++) {
                if (strchr(operands->array[k], '(')) { 
                    calcluate_aggregated_field(operands, record, new, result, k, j);
                }
                else if (!strcmp(result->fields->array[j], operands->array[k])) {
                    add_to_arraylist(new, strdup(record->array[j]));
                }
            }
        }
        add_to_arraylist(output->records, new);
    }
    free_table(result, inds);
    return output;
}