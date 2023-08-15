#ifndef AST_H
#define AST_H
#include "arraylist.h"
#include "hashtable.h"
#include <stdbool.h>
#include <string.h>
typedef enum {
    NUMERIC,
    CHARACTER,
} VariableType;

typedef struct {
    VariableType variable_type;
    int number;
    char* string;
} VariableData;

typedef enum {
    AND,
    OR,
    NOT,
    NOTEQUAL,
    EQUAL,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    LESSTHAN,
    GREATERTHAN,
    VARIABLE,
    NUM,
    GREATEREQUAL,
    LESSEQUAL,
    STRINGLITERAL,
} Type;

typedef struct AST AST;
typedef union Node Node;
typedef struct BinOpp BinOpp;

// Definition of the structures
struct BinOpp {
    AST* left;
    AST* right;
};

union Node {
    BinOpp bin_opp;
    char* variable;
    int number;
    char *literal;
};

struct AST {
    Type type;
    Node node;
};

void free_ast(AST*); 
AST* make_ast(Type, AST*, AST*);
AST* make_number(int);
AST* make_variable(char*);
AST* make_literal(char*);
int evaluate_ast(AST*, HashTable);
void get_variables(AST*, ArrayList*);
void free_variable_data(VariableData*);
VariableData* init_variable_data();
#endif