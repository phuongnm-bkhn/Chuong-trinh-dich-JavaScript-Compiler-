#ifndef C_LEXER_H
#define C_LEXER_H

#include <iostream> 
#include <vector>
#include <map>
using namespace std;

enum TOKEN_ID
{
    // Kieu du lieu co ban
    ID_VAR = 1,
    STRING,
    NUMBER,

    // cac tu khoa co ban (reserve string)
    TOKEN_R_DO,
    TOKEN_R_WHILE,
    TOKEN_R_IF,
    TOKEN_R_THEN,
    TOKEN_R_ELSE,
    TOKEN_R_VAR,
    TOKEN_R_FUNCTON,
    TOKEN_R_RETURN,
    TOKEN_R_TRUE,
    TOKEN_R_FALSE,

    // cac toan tu co ban (opmath)
    // char    [(){}\[\];]
    TOKEN_CHAR_open_1,
    TOKEN_CHAR_close_1,
    TOKEN_CHAR_open_2,
    TOKEN_CHAR_close_2,
    TOKEN_CHAR_open_3,
    TOKEN_CHAR_close_3,
    TOKEN_CHAR_END_STATEMENT,

    // cac toan tu co ban (opmath): +... ?:
    TOKEN_OP_PLUS,
    TOKEN_OP_PLUSPLUS,
    TOKEN_OP_MINUS,
    TOKEN_OP_MINUSMINUS,
    TOKEN_OP_DIV,
    TOKEN_OP_MULTI,
    TOKEN_OP_EQUAL,
    TOKEN_OP_NOT,

    // cac so sanh co ban (opmath): ><
    TOKEN_CONDITION_GT,
    TOKEN_CONDITION_GTEQUAL,
    TOKEN_CONDITION_LT,
    TOKEN_CONDITION_LTEQUAL,
    TOKEN_CONDITION_EQUALEQUAL,

    TOKEN_EOF
};

vector<string> getListToken(string sPathFileCode);
vector<string> getListToken1(string sPathFileCode);
extern std::map<TOKEN_ID, std::string> g_mapTokenName;

#endif