%{
#include <stdio.h>
#include "ast.h"

extern int yylex();
extern int yyparse();

AST* parsed_expression;

int yyerror(char *msg) {
    printf("Error: %s\n", msg);
    return 0;
}

int yywrap() {
    return 1; // Indicate end of input stream
}

void test(AST* hi) {
    return;
}

%}

%union {
    AST* value;
}

%type <value> LITERAL NUMBER STRING exp line calc
%token NUMBER
%token STRING
%token LITERAL
%left LOGICAL_OR
%left LOGICAL_AND
%left LOGICAL_NOT
%left NEQ EQ
%left '<' '>' GEQ LEQ
%left '+' '-'
%left '*' '/'
%left UMINUS
%%

calc:   /* empty */
        | calc line '\n' { parsed_expression = $2; } // Save the result to 'result' variable
        | calc '\n'
        ;

line:   exp           { $$ = $1; }
    ;

exp:  NUMBER                 { $$ = $1; }
    | STRING                 { $$ = $1; }
    | LITERAL                { $$ = $1; }
    | exp '+' exp            { $$ = make_ast(PLUS, $1, $3); }
    | exp '-' exp            { $$ = make_ast(MINUS, $1, $3); }
    | exp '*' exp            { $$ = make_ast(MULTIPLY, $1, $3); }
    | exp '/' exp            { $$ = make_ast(DIVIDE, $1, $3); }
    | exp LOGICAL_AND exp    { $$ = make_ast(AND, $1, $3); }
    | exp LOGICAL_OR exp     { $$ = make_ast(OR, $1, $3); } 
    | LOGICAL_NOT exp        { $$ = make_ast(NOT, NULL, $2); } 
    | exp '<' exp            { $$ = make_ast(LESSTHAN, $1, $3); } 
    | exp '>' exp            { $$ = make_ast(GREATERTHAN, $1, $3); } 
    | exp NEQ exp            { $$ = make_ast(NOTEQUAL, $1, $3); } 
    | exp GEQ exp            { $$ = make_ast(GREATEREQUAL, $1, $3); } 
    | exp LEQ exp            { $$ = make_ast(LESSEQUAL, $1, $3); } 
    | exp EQ exp             {$$ = make_ast(EQUAL, $1, $3); }
    | '-' exp %prec UMINUS   { $$ = make_ast(MINUS, NULL, $2); }
    | '(' exp ')'            { $$ = $2; } // Grouping with brackets
    ;
%%
