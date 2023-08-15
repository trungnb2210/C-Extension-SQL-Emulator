#include "utils.h"
#include "ast.h"
#include "aggregates.h"
#include "ctype.h"


/*
If the current variable is of the form FUNCTION(COLUMN) then this function is called
It extracts the column name and creates a numeric variable that contains
the reduced value of the arraylist held at that record
*/
static void create_aggregated_variable(Table* result, ArrayList* variables, ArrayList* record, VariableData* variable_data, int variable_index) {
    char *ptr = strdup(variables->array[variable_index]);
    char *p = strtok(ptr, "(");
    p = strtok(NULL, ")");
    char *newp = strdup(variables->array[variable_index]);
    newp = strtok(newp, "(");
    char* element = (*parse_agg(newp))(record->array[find_attribute_index(result, p)]);
    variable_data->variable_type = NUMERIC;
    variable_data->number = atoi(element); 
    free(newp);
    free(ptr);
    free(element);
}

/*
If the current variable is a normal variable then it inspects whether the element
stored at that index is an integer and thus creates creates an number that
will be inserted to the mapping and a string otherwise
*/
static void create_non_aggregated_variable(Table* result, ArrayList* variables, ArrayList* record, VariableData* variable_data, int variable_index) {
    char *element = strdup((char*)record->array[find_attribute_index(result, variables->array[variable_index])]);
    if (isdigit(element[0])) {
        variable_data->variable_type = NUMERIC;
        variable_data->number = atoi(element);
        free(element);
    } else {
        variable_data->variable_type = CHARACTER;
        variable_data->string = element;
    }
}

/*
Builds the mapping from variable names -> variable values that will be used
in the AST evaluation function
Has to differentiate between variables of the form FUNC(COLUMN) or COLUMN
*/
static HashTable build_mapping(Table* result, ArrayList *variables, ArrayList* record) {
    HashTable mapping = initialise_hash_table();
    for (int j = 0; j < variables->size; j++) {
        VariableData* variable_data = init_variable_data();
        if (strchr(variables->array[j], '(') != NULL) {
            create_aggregated_variable(result, variables, record, variable_data, j);
        } else { 
            create_non_aggregated_variable(result, variables, record, variable_data, j);
        }

        insert(mapping, variables->array[j], variable_data);
    }
    return mapping;
}

/*
A mapping is built from variable names -> variable values and is used
to evaluate the value of the abstract syntax tree for this specific record
if the condition is satisfied then it is added to the new Table
*/
static void filter_rows(Table* result, Table* output, ArrayList* variables, ArrayList* inds) {
    for (int i = 0; i < result->records->size; i++) {
        ArrayList* record = result->records->array[i];
        HashTable mapping = build_mapping(result, variables, record);
        if (evaluate_ast(parsed_expression, mapping)) {
            ArrayList *toadd = initialize_arraylist();
            for (int j = 0; j < record->size; j++) { 
                if (inds == NULL) { 
                    add_to_arraylist(toadd, strdup((char *) record->array[j]));
                } else {
                    if (value_in_arr(inds, j)) {
                        add_to_arraylist(toadd, strdup((char *)record->array[j]));  
                    } else {
                        add_to_arraylist(toadd, make_array_copy(record->array[j])); 
                    }
                } 
            }
        add_to_arraylist(output->records, toadd);
    } 
    free_hash_table(mapping, 0);
    }
}

/*
Frees the memory used to parse where and having and deletes the buffer
that was created for the parser
*/
static void free_memory_buffer(YY_BUFFER_STATE buffer, ArrayList* variables, Table* result, ArrayList* inds) {
    yy_delete_buffer(buffer);
    free_array_list(variables); 
    free_table(result, inds);
    free_ast(parsed_expression);  
}

/*
Builds a new table with the same field names, then parse the expression held in 
the first operand using the parser.y and parser.l bison and flex files and
stores the resulting AST in parsed_expression 
A mapping is built from variable names -> values which is used to evaluate the ast
and it is filters rows on whether they satisfy the expression
*/
Table* parse_where_having(Table* result, ArrayList* operands, ArrayList* inds) {
    if (operands == NULL)
        return result;
    Table* output = init_table();
    transfer_elements(output->fields, result->fields);
    
    YY_BUFFER_STATE buffer = yy_scan_string(operands->array[0]);
    yyparse();

    ArrayList *variables = initialize_arraylist();
    get_variables(parsed_expression, variables);

    filter_rows(result, output, variables, inds);
    
    free_memory_buffer(buffer, variables, result, inds);
    return output;
}