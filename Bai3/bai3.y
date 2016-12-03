%{
#include<stdio.h>
#include<string.h>
#include <stdlib.h>

#include"bai3.tab.h"

extern FILE *yyin;

void yyerror(const char *str);
%}

%token ID NUMBER WHILE DO CONDITION OR AND VAR
%left AND OR CONDITION
%start S
%%

S : S1 S
    | S1

S1 : S_W                   { printf("Xâu while.\n");  }
    |S_DW                { printf("Xâu do-while.\n");  }     
    |statement
    ;

S_DW : DO block WHILE '(' condition ')' ';' ;

S_W    : WHILE '(' condition ')' block ;

block : '{' statement_list '}'

statement_list : /* blank */ 
                    | statement statement_list
                    ;

statement : declaration
                    | assignment
                    | ex ';' ;

declaration : VAR assignment                                        { printf("Định nghĩa - "); }

assignment : ID '=' ex ';'                                                    { printf("Gán giá trị.\n");  } 

ex : ex '+' ex
    | ex '-' ex
    | ex '*' ex
    | ex '/' ex
    | ID
    | NUMBER
    ;

condition : condition OR condition
                | condition AND condition
                | single_condition

single_condition : ex CONDITION ex
                                | ID
                                | NUMBER
                                ;
%%

int main(void){
    //printf("Nhập chuỗi:\n");
    //yyparse();

    int c;
    FILE *file;
    file = fopen("test_input.js", "r");
    printf("\n\n");    
    printf("Mã nguồn:");
    printf("\n\n");    
    if (file) {
            while ((c = getc(file)) != EOF)
                putchar(c);
            fclose(file);
    }
    printf("\n\n");
    printf("Nhận diện: ");
    printf("\n\n");
    yyin=fopen("test_input.js","r");
    yyparse();
    //yylex();
    printf("\n");
}

void yyerror(const char *str)
{
        printf("error: %s\n",str);
        exit(1);
}