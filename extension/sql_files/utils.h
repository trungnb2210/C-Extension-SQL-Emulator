#ifndef UTILS_H
#define UTILS_H
#include "table.h"
#include "arraylist.h"
#include "ast.h"
#include <stdio.h>


#define IS_RECORD 1
#define NOT_RECORD 0
#define NO_INDICES NULL
#define MAX(x,y) ((x > y) ? x : y)
#define MIN(x,y) ((x < y) ? x : y)

typedef struct yy_buffer_state* YY_BUFFER_STATE;
extern int yyparse();
extern YY_BUFFER_STATE yy_scan_string(char * str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);
int yylex_destroy(void);
extern AST* parsed_expression;


typedef enum {
    TABLE_NAME = 0,
    FIELDS = 1,
    RECORDS = 2,
} ParsingState;

typedef enum {
    SELECT = 0,
    FROM = 1,
    WHERE = 2,
    ORDERBY = 3,
    GROUPBY = 4,
    HAVING = 5
} Commands;

int find_attribute_index(Table*, char*);
bool value_in_arr(ArrayList*, int);
void transfer_elements(ArrayList*, ArrayList*);
ArrayList* make_array_copy(ArrayList*);
ArrayList* make_record_copy(ArrayList*);
void free_records(ArrayList*, ArrayList*);

#endif