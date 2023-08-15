#include "table.h"
#include "utils.h"
#include "utils.h"

/*
Dynamically allocates the memory required for a table, which
contains two arraylists one for the records (nested ArrayList) and one for the 
field
*/
Table* init_table(void) {
    Table* table = malloc(sizeof(Table));
    table->records = initialize_arraylist();
    table->fields = initialize_arraylist();
    return table;
}

/*
Makes a copy of the table
*/
Table* make_table_copy(Table* original) {
    Table* copy = malloc(sizeof(Table));
    copy->records = make_record_copy(original->records);
    copy->fields = make_array_copy(original->fields);
    return copy;
}

/*
Frees the dynamically allocated memory for the table
*/
void free_table(Table* table, ArrayList* inds) {
    free_records(table->records, inds);
    free_array_list(table->fields);
    free(table);
}
