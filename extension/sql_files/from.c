#include "from.h"
#include "utils.h"
#include "utils.h"

/*
The original command will have been of the form FROM table1, table2, ...
This populates the associated tables from the names of the tables
then calculates the cartesian product of them
*/
Table* parse_from(HashTable tables, ArrayList* operands) {
    Table *to_product[operands->size];
    for (int i = 0; i < operands->size; i++) {
        to_product[i] = get(tables, operands->array[i]);
    }
    return product(to_product, operands->size);
}

/*
Given an array of tables (and the size) we calculate the 
cartesian product of all the tables
The fields are dealt with by just adding them on for each table
as (our SQL-- doesn't support overlapping field names) for the records
are built up by adding the current record to each of the records that we have
already added. Then seting the new list of records to this combination of records
As this is performed for each table, every combination of records will be
achieved
*/
Table* product(Table* tables[], int table_size) {
    Table* result = make_table_copy(tables[0]);

    for (int i = 1; i < table_size; i++) {
        ArrayList* records = initialize_arraylist();
        transfer_elements(result->fields, tables[i]->fields);

        for (int j = 0; j < result->records->size; j++) {
            ArrayList* row1 = result->records->array[j];
            for (int k = 0; k < tables[i]->records->size; k++) {
                ArrayList *new = make_array_copy(row1);
                transfer_elements(new, tables[i]->records->array[k]);
                add_to_arraylist(records, new);
            }
        }
        free_records(result->records, NO_INDICES);
        result->records = records;
    }
    return result;
}