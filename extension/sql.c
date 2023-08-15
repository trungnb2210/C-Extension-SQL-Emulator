#include "utils.h"
#include "arraylist.h"
#include "hashtable.h"
#include <stdlib.h>
#include <ctype.h>
#include "ast.h"
#include "parser.tab.h"
#include "groupby.h"
#include "aggregates.h"
#include "arraylist.h"
#include "from.h"
#include "orderby.h"
#include "wherehaving.h"
#include "join.h"
#include "select.h"
#include <assert.h>

#define NUMBER_COMMANDS 6
#define IS_TABLE 1
#define MAX_LINE_LENGTH 256
#define TABLE_LOCATION 1
#define QUERY_LOCATION 2
#define RESULT_LOCATION 3


/*
Prototype declarations for static functions
*/
static void parse_values(char*, ArrayList*);
static void parse_table(HashTable, char*);
static int parse_index(char *);
static ArrayList** parse_query(HashTable, char*);
static void print_table(FILE *, ArrayList *, ArrayList *);
static void write_output(Table*, char*);
static HashTable read_tables(char*);
static ArrayList** read_query(HashTable, char*);
static Table* perform_queries(HashTable, ArrayList**);
static void free_memory(HashTable, ArrayList**, Table*);

/*
Main function which process the tables, the query, each command 
and writes the output 
These are all stored in .txt files with the location specified by the
command line arguments
*/
int main(int argc, char *argv[]) {
    assert(argc == 4);
    HashTable tables = read_tables(argv[TABLE_LOCATION]);
    ArrayList** commands = read_query(tables, argv[QUERY_LOCATION]);
    Table* result = perform_queries(tables, commands);
    write_output(result, argv[RESULT_LOCATION]);
    free_memory(tables, commands, result);

}

/*
Splits the user input using the specified delimiters
If the last token was ON it means it will be followed by a logical expression
so do we not want to split on " "
*/
static void parse_values(char* buffer, ArrayList* array) {
  const char delim[] = " ,\n";
  char *token;
  char *save_ptr;
  token = strtok_r(buffer, delim, &save_ptr);
  while (token !=  NULL) {
    add_to_arraylist(array, strdup(token));
    token = strtok_r(NULL, delim + !strcmp(token, "ON"), &save_ptr);
  }
}

/*
Reads the table name, fields, and records from the table at the file location
*/
static void parse_table(HashTable tables, char* filename) {
    FILE *file = fopen(filename, "r");
    char buffer[MAX_LINE_LENGTH];
    char name[MAX_LINE_LENGTH];
    ArrayList *record;
    Table* table = init_table();
    ParsingState parsing_state = TABLE_NAME;

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (!strcmp(buffer, "\n")) {
            if (parsing_state != TABLE_NAME) {
                insert(tables, name, table);
                table = init_table();
                parsing_state = TABLE_NAME;;
            }
        } else {
            switch (parsing_state) {
                case TABLE_NAME: {
                    strcpy(name, buffer);
                    name[strlen(name) - 2] = '\0';
                    parsing_state++;
                    break;
                }
                case FIELDS: {
                    parse_values(buffer, table->fields);
                    parsing_state++;
                    break;
                }
                case RECORDS: {
                    record = initialize_arraylist();
                    parse_values(buffer, record);
                    add_to_arraylist(table->records, record);
                    break;
                }
            }
        }
    }
    free_table(table, NO_INDICES); //last one is initialised but unused
    fclose(file);
}

/*
Returns the index where you can find the operands for each of the commands
that we use
*/
static int parse_index(char *command) {
    if (!strcmp(command, "SELECT")) {
        return SELECT;
    } else if (!strcmp(command, "FROM")) {
        return FROM;
    } else if (!strcmp(command, "ORDER")) {
        return ORDERBY;
    } else if (!strcmp(command, "WHERE")) {
        return WHERE;
    } else if (!strcmp(command, "GROUP")) {
        return GROUPBY;
    } else if (!strcmp(command, "HAVING")) {
        return HAVING;
    } else {
        exit(EXIT_FAILURE);
    }
}



/*
Parses each line of the query at the specified file location
If the command is WHERE or HAVING then it will contain a logical expression
afterwards, therefore we want to keep us this as it is for the parser
*/
static ArrayList** parse_query(HashTable tables, char* filename)  {
    FILE *file = fopen(filename, "r");
    char buffer[MAX_LINE_LENGTH];
    char command[MAX_LINE_LENGTH];
    ArrayList** operands = calloc(NUMBER_COMMANDS, sizeof(ArrayList *));
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        sscanf(buffer, "%s", command);
        operands[parse_index(command)] = initialize_arraylist();
        if (!strcmp(command, "WHERE") || !strcmp(command, "HAVING")) {
            add_to_arraylist(operands[parse_index(command)], strdup(buffer + strlen(command) + 1));
        } else {
            parse_values(buffer + strlen(command), operands[parse_index(command)]);
        }

    }
    fclose(file);
    return operands;
}

/*
Prins the result table into the specified FILE accounting for padding
and adding borders for clarity
*/
static void print_table(FILE *file, ArrayList *headers, ArrayList *records) {
    // create arraylist of max length for each attribute 
    int attribute_lengths[headers->size];
    int table_length = headers->size + 1;
    int index = 0;
    for (int i = 0; i < headers->size; i++) { // same size for all 
        int curr_len = 0;
        // check header length
        curr_len = MAX(curr_len, strlen((char *) headers->array[i]));
        for (int j = 0; j < records->size; j++) {
            curr_len = MAX(curr_len, strlen((char *) ((ArrayList *) records->array[j])->array[i]));
        }
        attribute_lengths[index++] = curr_len;
        table_length += curr_len + 2;
    }
    // maxlens of all attributes initialised 

    for (int i = 0; i < headers->size; i++) {
        fprintf(file, "  %s", ((char*) headers->array[i]));
        for (int j = 0; j < 1 + attribute_lengths[i] - strlen((char *) headers->array[i]); j++) {
            fprintf(file, " ");
        } // padding
    }
    fprintf(file, "\n");

    for (int i = 0; i < table_length; i++) {
        fprintf(file, "-");
    }
    fprintf(file, "\n"); // underlines with padded

    for (int i = 0; i < records->size; i++) {
        ArrayList *curr_row = records->array[i];
        fprintf(file, "| ");
        for (int j = 0; j < curr_row->size; j++) {
            fprintf(file, "%s", ((char*) curr_row->array[j]));
            for (int k = 0; k < attribute_lengths[j] - strlen(curr_row->array[j]); k++) {
                fprintf(file, " ");
            }
            fprintf(file, " | ");
        }
        fprintf(file, "\n");
    }
    for (int i = 0; i < table_length; i++) {
        fprintf(file, "-");
    }
    fprintf(file, "\n"); // underlines with padded
}

/*
Writes the result table into the specified file location
*/
static void write_output(Table* result, char* filename) {
    FILE *file = fopen(filename, "w");
    fprintf(file, "QUERY RESULT:\n");
    print_table(file, result->fields, result->records);
    fclose(file);
}


/*
Returns a hashtable which has a mapping between the table names and the tables
themselves
*/
static HashTable read_tables(char* filename) {
    HashTable tables = initialise_hash_table();
    parse_table(tables, filename);
    return tables;
}

/*
Used to read all the lines in queries and stores in an array of arraylists,
where each index is for a different command
It will also reduce joins to a new table that we create
*/
static ArrayList** read_query(HashTable tables, char* filename) {
    ArrayList** commands = parse_query(tables, filename);
    ArrayList* temp_from = parse_join(tables, commands[FROM]);
    free_array_list(commands[FROM]);
    commands[FROM] = temp_from;
    return commands;
}

/*
Performs all the commands in the correct order of operations
returning the resultant table
*/
static Table* perform_queries(HashTable tables, ArrayList** commands) {
    Table* result;

    result = parse_from(tables, commands[FROM]);
    ArrayList* indices = initialize_arraylist();
    result = parse_where_having(result, commands[WHERE], NULL);
    result = parse_group(result, commands[GROUPBY], indices);
    result = parse_where_having(result, commands[HAVING], indices);
    result = parse_select(result, commands[SELECT], indices);
    result = parse_order(result, commands[ORDERBY]);
    free_array_list(indices);
    return result;

}

/*
Frees the dynamically allocated memory that is used by the main function
Also destroys the buffer that is created for parsing
*/
static void free_memory(HashTable tables, ArrayList** commands, Table* result) {
    free_hash_table(tables, IS_TABLE);
    for (int i = 0; i < NUMBER_COMMANDS; i++) {
        if (commands[i] != NULL) 
            free_array_list(commands[i]);
    }
    free(commands);
    free_table(result, NULL);
    yylex_destroy();
}


