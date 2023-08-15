%{
#include "ast.h"
#include "parser.tab.h"
%}

%option noinput
%option nounput

%%

[ \t]       /* Ignore whitespace */
\n          { return '\n'; }
[0-9]+      { yylval.value = make_number(atoi(yytext)); return NUMBER; }
\"([^\"\\]|\\.)*\" {yylval.value = make_literal(strdup(yytext + 1)); return LITERAL; }
"("         { return '('; }
")"         { return ')'; }
"+"         { return '+'; }
"-"         { return '-'; }
"*"         { return '*'; }
"/"         { return '/'; }
"<"         { return '<';}
">"         { return '>';}
"!="        { return NEQ;}
"=="        { return EQ;}
"AND"       { return LOGICAL_AND; }
"OR"        { return LOGICAL_OR; }
">="        { return GEQ; }
"<="        { return LEQ; }
"NOT"       { return LOGICAL_NOT; }
([a-zA-Z]+)|([A-Z]+\([a-zA-Z]+\))   { yylval.value = make_variable(strdup(yytext)); return STRING; }
.           { return yytext[0]; } /* Return other single characters as is */
%%