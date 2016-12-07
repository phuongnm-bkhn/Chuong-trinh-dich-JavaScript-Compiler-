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
%left AND OR
%start S
%%

// luat sinh nay cho phep phan tich nhieu khoi lenh noi tiep nhau
// (neu ko co luat sinh nay ma viet truc tiep S :S_W | S_DW | statement ... o luat sinh tiep theo)
// (khi do trong vi du o file test_input.js; yacc parse se dung lai sau ham do_while dau tien)

S : S1
    | S S1

// 1 khoi lenh S1 ( 1 khoi can phan tich ) co the la 1 khoi while, 1 khoi do-while hoac chi la 1 statement don le.

S1 : S_W                   { printf("Xâu while.\n");  }
    |S_DW                { printf("Xâu do-while.\n");  }     
    |statement    

// 1 khoi lenh do-while hoan chinh se bao gom "DO" + khoi lenh trong do-while + "WHILE" + dieu kien ...

S_DW : DO block WHILE '(' condition ')' ';'

// 1 khoi lenh while hoan chinh se bao gom "WHILE" + ( dieu kien ) + khoi lenh trong while ...

S_W    : WHILE '(' condition ')' block ;

// khoi lenh trong while va do-while bao gom nhieu statement, bao boc boi { }
block : '{' statement_list '}'

// statement_list co the blank, 1 statement, hoac bao gom nhieu statement noi tiep nhau
// vi o day da co blank nen ko can viet "statement | statement_list statement" giong nhu luat sinh so 1

statement_list : /* blank */
                | statement_list statement

// 1 statement co the la 1 dinh nghia hoac la 1 lenh gan gia tri

statement : declaration                         
            | assignment            

// dinh nghia la 1 lenh gan gia tri co var phia truoc :D

declaration : VAR assignment    { printf("Định nghĩa - "); }

// gan gia tri tuc la ID = phep toan

assignment : ID '=' ex ';'       { printf("Gán giá trị.\n");  }

ex : ex '+' ex
    | ex '-' ex
    | ex '*' ex
    | ex '/' ex
    | ID
    | NUMBER    

// ham nay cho phep noi nhieu dieu kien voi nhau bang or hoac and

condition : condition OR condition
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