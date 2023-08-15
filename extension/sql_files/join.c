#include "join.h"
#include "table.h"
#include "wherehaving.h"
#include <string.h>
#include <stdio.h>
#include "from.h"
#define NEW_TABLE_NAME_LENGTH 10
#define TABLE1_OFFSET 0
#define JOIN_OFFSET 1
#define TABLE2_OFFSET 2
#define EXPRESSION_OFFSET 4
#define JOIN_LENGTH 4
#define NUMBER_OF_TABLES 2
#define EXTRA_NEW_LINE_SPACE 2
/*
This will perform table1 JOIN table2 ON expr
This is done by taking the product of table1 and table2 and filtering for
the records which match the predicate expr
The index is passed in order to ensure a unique table for this new table
we have generated from the join and this new table is added to our tables
hashtable under that name
*/
static char* perform_join(HashTable tables, int index, char* table1, char* table2, char* expression) {
    char* name = malloc(NEW_TABLE_NAME_LENGTH * sizeof(char));
    sprintf(name, "%d", index);
    Table* new_table = product((Table*[]) {get(tables, table1), get(tables, table2)}, NUMBER_OF_TABLES);
    ArrayList* temp = initialize_arraylist();
    char* expression_newline = strcpy(malloc(strlen(expression) + EXTRA_NEW_LINE_SPACE), expression);
    strcat(expression_newline, "\n");
    add_to_arraylist(temp, expression_newline);
    Table* filtered_table = parse_where_having(new_table, temp, NULL);
    insert(tables, name, filtered_table);
    free_array_list(temp);
    return name;
}

/*
The original command will be of the form FROM <regular table>/<A JOIN B ON C>, ...
If a JOIN is not contained is the subsequent operand then it can be simply
added back to the operands without further processing
In the where the next 4 elements are A, JOIN, B, ON, C, then a join needs to be
performed between A and B
So the significant operands need to passed into the perform join function
*/
ArrayList* parse_join(HashTable tables, ArrayList* operands) {
    ArrayList* output = initialize_arraylist();
    for (int i = 0; i < operands->size; i++) {
        if (i + JOIN_LENGTH < operands->size && !strcmp(operands->array[i+JOIN_OFFSET], "JOIN")) {
            add_to_arraylist(output, 
                perform_join(tables, i, 
                    operands->array[i+TABLE1_OFFSET], operands->array[i+TABLE2_OFFSET], operands->array[i+EXPRESSION_OFFSET]));
            i += JOIN_LENGTH;
        } else {
            add_to_arraylist(output, strdup(operands->array[i]));
        }
    }
    return output;
}