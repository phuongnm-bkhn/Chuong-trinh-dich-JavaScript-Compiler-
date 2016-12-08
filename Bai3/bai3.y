%{
#include<stdio.h>
#include<string.h>
#include <stdlib.h>

#include"bai3.tab.h"

extern FILE *yyin;

int yylex();
void yyerror(const char *str);

%}

%token ID NUMBER WHILE DO CONDITION OR AND VAR
%left AND OR "-" "+" "*" "/"
%start S

%union {
    char *s;    
}
%type <s> S_W S_DW block assignment statement declaration statement_list
%%

// luat sinh nay cho phep phan tich nhieu khoi lenh noi tiep nhau
// (neu ko co luat sinh nay ma viet truc tiep S :S_W | S_DW | statement ... o luat sinh tiep theo)
// (khi do trong vi du o file test_input.js; yacc parse se dung lai sau ham do_while dau tien)

S : S1                  
   |S1 S


// 1 khoi lenh S1 ( 1 khoi can phan tich ) co the la 1 khoi while, 1 khoi do-while hoac chi la 1 statement don le.

S1 : S_W                           
    |S_DW    
    |statement
    { printf("- Cau lenh don \n");}

// 1 khoi lenh do-while hoan chinh se bao gom "DO" + khoi lenh trong do-while + "WHILE" + dieu kien ...

S_DW : DO block WHILE '(' condition ')' ';'     { printf("- Khoi do-while \n"); printf("%s \n", $2); }

// 1 khoi lenh while hoan chinh se bao gom "WHILE" + ( dieu kien ) + khoi lenh trong while ...

S_W    : WHILE '(' condition ')' block          { printf("- Khoi while \n"); printf("%s \n", $5); }


// khoi lenh trong while va do-while bao gom nhieu statement, bao boc boi { }
block : '{' statement_list '}'                  { $$ = $2; }

// statement_list co the blank, 1 statement, hoac bao gom nhieu statement noi tiep nhau
// vi o day da co blank nen ko can viet "statement | statement_list statement" giong nhu luat sinh so 1

statement_list : /* blank */                    
                { $$ = ""; }
                | statement statement_list
                { 
                    char *str = (char*) malloc(strlen($1) + strlen($2) + 1);
                    strcpy(str, $1);
                    strcat(str, "\n");
                    strcat(str, $2);
                    $$ = str;
                }

// 1 statement co the la 1 dinh nghia hoac la 1 lenh gan gia tri

statement : declaration                         { $$ = $1; }
            | assignment                        { $$ = $1; }

// dinh nghia la 1 lenh gan gia tri co var phia truoc :D

declaration : VAR assignment                    { $$ = "--- lenh khoi tao"; }

// gan gia tri tuc la ID = phep toan

assignment : ID '=' ex ';'                      { $$ = "--- lenh gan gia tri"; }

ex : ex '+' ex
    | ex '-' ex
    | ex '*' ex
    | ex '/' ex
    | ID
    | NUMBER    

// ham nay cho phep noi nhieu dieu kien voi nhau bang or hoac and
// trong condition cac dieu kien co the nam trong "( )" hoac khong
// vi du: a && b || c
// hoac la : a && (b || c)

condition : condition OR condition
        | "(" condition OR condition ")"
        | "(" condition AND condition ")"
        | condition AND condition
        | single_condition             

// 1 phep so sanh co the la dieu kien (a <= b) hoac la 1 bien (x) hoac la 1 so gi do (1)

single_condition : ex CONDITION ex
                | ID
                | NUMBER
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