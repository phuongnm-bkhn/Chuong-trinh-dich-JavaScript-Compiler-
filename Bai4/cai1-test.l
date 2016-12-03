/* scanner for a toy Pascal-like language */
%option noyywrap

%{
/* need this for the call to atof() below */
#include <math.h>

%}

alpha	[A-Za-z]*
dig	[0-9]

%%

{dig}	printf("\ndig [%s]", yytext);

{alpha}	printf("\nalpha [%s]", yytext);

%%

int main( int argc, char **argv )
{
yyin = fopen( "test.txt", "r" );
yylex();

getchar();
return 0;
}