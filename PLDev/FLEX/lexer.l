%{
    /* C code, headers, and definitions */
#include <stdio.h>
%}
    
%%


[ \t\n]+           ;           // Ignore whitespace
"if"               return IF;
"else"             return ELSE;
[a-zA-Z_][a-zA-Z0-9_]*  return IDENTIFIER;
[0-9]+             return NUMBER;
"+"                return PLUS;
"-"                return MINUS;
.                  return yytext[0]; // return character for unknown symbols

%%

/* User code: main function, helper functions, etc. */
int yywrap() {
    return 1;
}
