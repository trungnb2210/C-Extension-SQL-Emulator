#include "ast.h"
#include "stdlib.h"
#include <stdio.h>

/*
Recursively frees the abstract syntax tree
Different conditions depending on the type of ast and the type stored in the node
*/
void free_ast(AST* ast) {
    if (ast->type == VARIABLE) {
        free(ast->node.variable);
    } else if (ast->type == STRINGLITERAL) {
        free(ast->node.literal);
    } else if (ast->type != NUM) {
        if (ast->node.bin_opp.left != NULL)
            free_ast(ast->node.bin_opp.left);
        free_ast(ast->node.bin_opp.right);
    }
    free(ast);
}

/*
Makes an internal abstract syntax tree node with a type (operation it performs) 
and two children
*/
AST* make_ast(Type type, AST* left, AST* right) {
    AST* ast = malloc(sizeof(AST));
    ast->type = type;
    ast->node = (Node) {.bin_opp = {.left = left, .right = right}};
    return ast;
}

/*
Creates a leaf node in the abstract syntax tree which contains a number
*/
AST* make_number(int num) {
    AST* ast = malloc(sizeof(AST));
    ast->type = NUM;
    ast->node = (Node) {.number = num};
    return ast;
}

/*
Creates a leaf node in the abstract syntax tree which contains a literal
*/
AST* make_literal(char *literal) {
    AST* ast = malloc(sizeof(AST));
    ast->type = STRINGLITERAL;
    literal[strlen(literal) - 1] = '\0';
    ast->node = (Node) {.literal = literal};
    return ast;
}

/*
Creates a leaf node in the abstract syntax tree which contains a variable
The value of this variable is determined later on by the mapping
*/
AST* make_variable(char* variable) {
    AST* ast = malloc(sizeof(AST));
    ast->type = VARIABLE;
    ast->node = (Node) {.variable = variable};
    return ast;
}

/*
Adds all the variables present in the abstract syntax tree (the leaf nodes)
to an arraylist
*/
void get_variables(AST* ast, ArrayList* array) {
    if (ast->type == VARIABLE) {
        add_to_arraylist(array, strdup(ast->node.variable));
    } else if (ast->type != NUM && ast->type != STRINGLITERAL) {
        if (ast->node.bin_opp.left != NULL)
            get_variables(ast->node.bin_opp.left, array);
        get_variables(ast->node.bin_opp.right, array);
    }
}

/*
Helper functions which determines whether the two leaf nodes currently being 
processed are strings
This is needed as the as the evalute_ast function returns an integer
Therefore we cannot have the string be propagated upwards

We can have strings if the variable either stores a string, or it is a string
literal
*/
static bool is_string(AST* ast, HashTable mapping) {
    if (ast == NULL) 
        return false;
    if (ast->type == STRINGLITERAL)
        return true;
    if (ast->type == VARIABLE) {
        VariableData* variable_data = get(mapping, ast->node.variable);
        return variable_data->variable_type == CHARACTER;
    }
    return false;
    
}

/*
If the is_string function is true then the function is returns the string 
associated with that respective leaf node
*/
static char* get_string(AST* ast, HashTable mapping) {
    if (ast ->type == STRINGLITERAL) {
        return ast->node.literal;
    } else {
        VariableData* variable_data = get(mapping, ast->node.variable);
        return variable_data->string;
    }
}


/*
The non-recusive function that deals with strings in the ast as the their string
value cannot be propagated up the tree due to return type
The operation performed on them is restricted from the normal node types
due to them being strings
*/
static int operation_on_strings(Type operation, AST* ast_left, AST* ast_right, HashTable mapping) {
    char *left_string = get_string(ast_left, mapping);
    char *right_string = get_string(ast_right, mapping);
    switch (operation) {
        case EQUAL:
            return !strcmp(left_string, right_string);
        case NOTEQUAL:
            return strcmp(left_string, right_string) != 0;
        case GREATERTHAN:
            return strcmp(left_string, right_string) > 0;
        case LESSTHAN:
            return strcmp(left_string, right_string) < 0;
        case LESSEQUAL:
            return strcmp(left_string, right_string) <= 0;
        case GREATEREQUAL:
            return strcmp(left_string, right_string) >= 0;
        default:
            exit(EXIT_FAILURE);
    }
    
}

/*
Helper function for the recursive evaluation of abstract syntax trees
This is the case where both the left and right subtrees are integers
The operation performed on them is dependent on the type of the ast
The NULL check for the left subtree is used for unary operations (NOT and MINUS)
*/
static int operation_on_ints(Type operation, BinOpp node, HashTable mapping) {
    int left = node.left == NULL ? 0 : evaluate_ast(node.left, mapping);
    int right = evaluate_ast(node.right, mapping);
    switch (operation) {
        case PLUS:
            return left + right;
        case MINUS:
            return left - right;
        case DIVIDE:
            return left / right;
        case MULTIPLY:
            return left * right;
        case NOT:
            return !right;
        case LESSTHAN:
            return left < right;
        case EQUAL:
            return left == right;
        case NOTEQUAL:
            return left != right;
        case AND:
            return left && right;
        case OR:
            return left || right;
        case GREATERTHAN:
            return left > right;
        case GREATEREQUAL:
            return left >= right;
        case LESSEQUAL:
            return left <= right;
        default:
            exit(EXIT_FAILURE);
    }
}

/*
Recurisve evaluation function of an abstract syntax tree
If the leaf node returns an integer then its returned
Otherwise, the function recursively evaluates the left and right subtrees
combining them with type/operation of the current node
Uses the is_string and get_string functionality as they can't be returned
so they must be dealt with without the recursion
*/
int evaluate_ast(AST* ast, HashTable mapping) {

    if (ast->type == VARIABLE) {
        VariableData* variable_data = get(mapping, ast->node.variable);
        return variable_data->number;
    } else if (ast->type == NUM) {
        return ast->node.number;
    }

    BinOpp node = ast->node.bin_opp;
    AST* ast_left = node.left;
    AST* ast_right = node.right;
    if (is_string(ast_left, mapping) && is_string(ast_right, mapping)) {
        return operation_on_strings(ast->type, ast_left, ast_right, mapping);
    }
    return operation_on_ints(ast->type, node, mapping);

}

/*
Function that frees variable data of a node, which is stored in the hashmap
As we need to distinguish between whether the variable is a string or an int
The string that is stored inside this data also has to be freed
as it will be dynamically allocated by strdup()
*/
void free_variable_data(VariableData* variable_data){
    if (variable_data->variable_type == CHARACTER) 
        free(variable_data->string);
    free(variable_data);
}

/*
Creates an instant of the struct variable data which allows us to differ
between the type of different variables (ints or strings)
*/
VariableData* init_variable_data() {
    VariableData* variable_data = malloc(sizeof(VariableData));
    return variable_data;
}