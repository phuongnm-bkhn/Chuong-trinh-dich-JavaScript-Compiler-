/* %option C++ noyywrap */
%option noyywrap
%{
#include<math.h>
#include<stdio.h>
#include<map>
#include<vector>
using namespace std;

int mylineno = 0;
enum TOKEN_ID {
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

std::map<TOKEN_ID, std::string> InitTokenName() {
        std::map<TOKEN_ID, std::string> mapTokenName;
                
        mapTokenName[TOKEN_ID::ID_VAR] = "id_var";
        mapTokenName[TOKEN_ID::STRING] = "string";
        mapTokenName[TOKEN_ID::NUMBER] = "number";
        
        mapTokenName[TOKEN_ID::TOKEN_R_DO] = "do";
        mapTokenName[TOKEN_ID::TOKEN_R_WHILE] = "while";
        mapTokenName[TOKEN_ID::TOKEN_R_IF] = "if";
        mapTokenName[TOKEN_ID::TOKEN_R_THEN] = "then";
        mapTokenName[TOKEN_ID::TOKEN_R_ELSE] = "else";
        mapTokenName[TOKEN_ID::TOKEN_R_VAR] = "var";
        mapTokenName[TOKEN_ID::TOKEN_R_FUNCTON] = "function";
        mapTokenName[TOKEN_ID::TOKEN_R_RETURN] = "return";
	mapTokenName[TOKEN_ID::TOKEN_R_TRUE] = "true";
	mapTokenName[TOKEN_ID::TOKEN_R_FALSE] = "false";
        
        mapTokenName[TOKEN_ID::TOKEN_OP_PLUS] = "+";
        mapTokenName[TOKEN_ID::TOKEN_OP_MINUS] = "-";
        mapTokenName[TOKEN_ID::TOKEN_OP_DIV] = "/";
        mapTokenName[TOKEN_ID::TOKEN_OP_MULTI] = "*";
        mapTokenName[TOKEN_ID::TOKEN_OP_EQUAL] = "=";
        mapTokenName[TOKEN_ID::TOKEN_CONDITION_GT] = ">";
        mapTokenName[TOKEN_ID::TOKEN_CONDITION_LT] = "<";
        mapTokenName[TOKEN_ID::TOKEN_CONDITION_GTEQUAL] = ">=";
        mapTokenName[TOKEN_ID::TOKEN_CONDITION_LTEQUAL] = "<=";
        mapTokenName[TOKEN_ID::TOKEN_CONDITION_EQUALEQUAL] = "==";

        mapTokenName[TOKEN_ID::TOKEN_CHAR_open_1] = "(";
        mapTokenName[TOKEN_ID::TOKEN_CHAR_open_2] = "[";
        mapTokenName[TOKEN_ID::TOKEN_CHAR_open_3] = "{";
        mapTokenName[TOKEN_ID::TOKEN_CHAR_close_1] = ")";
        mapTokenName[TOKEN_ID::TOKEN_CHAR_close_2] = "]";
        mapTokenName[TOKEN_ID::TOKEN_CHAR_close_3] = "}";

        mapTokenName[TOKEN_ID::TOKEN_CHAR_END_STATEMENT] = ";";
        return mapTokenName;
}

std::map<TOKEN_ID, std::string> g_mapTokenName = InitTokenName();
std::vector<TOKEN_ID> g_lstTokenDetected;


%}

string	((\"[^\n"]+\")|(\'[^\n']+\'))

ws	[ \t\r]+

alpha	[A-Za-z]
dig	[0-9]
var_name	({alpha}|\$)({alpha}|{dig}|\_|\.|\-|\/|\$)*

num1	{dig}+\.?([eE][-+]?{dig}+)?
num2	{dig}*\.{dig}+([eE][-+]?{dig}+)?
const_number	{num1}|{num2}

condition (==|>=|<=)
char    [(){}\[\];\+\-\/\*\=!><]

%%

{ws}	/* skip blanks and tabs */

"/*"		{/**/
		int c;

		while((c = yyinput()) != 0)
			{
			if(c == '\n')
				++mylineno;

			else if(c == '*')
				{
				if((c = yyinput()) == '/')
					break;
				else
					unput(c);
				}
			}
		}

"//"		{
		int c;

		while((c = yyinput()) != 0)
			{
			if(c == '\n'){
				++mylineno;
				break;	
			}		
			}
		}
"quit"          {g_lstTokenDetected.push_back(TOKEN_ID::TOKEN_EOF); return 0;}
"do" 		g_lstTokenDetected.push_back(TOKEN_ID::TOKEN_R_DO); //FLEX_STD cout << g_mapTokenName[TOKEN_ID::TOKEN_R_DO] << " << input: [" <<YYText() << "]\n";
"while"  	g_lstTokenDetected.push_back(TOKEN_ID::TOKEN_R_WHILE); //FLEX_STD cout << g_mapTokenName[TOKEN_ID::TOKEN_R_WHILE] << " << input: [" << YYText() << "]\n";
"if" 		g_lstTokenDetected.push_back(TOKEN_ID::TOKEN_R_IF); //FLEX_STD cout << g_mapTokenName[TOKEN_ID::TOKEN_R_IF] << " << input: [" << YYText() << "]\n";
"then"  	g_lstTokenDetected.push_back(TOKEN_ID::TOKEN_R_THEN); //FLEX_STD cout << g_mapTokenName[TOKEN_ID::TOKEN_R_THEN] << " << input: [" << YYText() << "]\n";
"else" 		g_lstTokenDetected.push_back(TOKEN_ID::TOKEN_R_ELSE); //FLEX_STD cout << g_mapTokenName[TOKEN_ID::TOKEN_R_ELSE] << " << input: [" << YYText() << "]\n";
"var" 		g_lstTokenDetected.push_back(TOKEN_ID::TOKEN_R_VAR); //FLEX_STD cout << g_mapTokenName[TOKEN_ID::TOKEN_R_VAR] << " << input: [" << YYText() << "]\n";
"function"	g_lstTokenDetected.push_back(TOKEN_ID::TOKEN_R_FUNCTON); //FLEX_STD cout << g_mapTokenName[TOKEN_ID::TOKEN_R_FUNCTON] << " << input: [" << YYText() << "]\n";
"return" 	g_lstTokenDetected.push_back(TOKEN_ID::TOKEN_R_RETURN); //FLEX_STD cout << g_mapTokenName[TOKEN_ID::TOKEN_R_RETURN] << " << input: [" << YYText() << "]\n";
"true" 		g_lstTokenDetected.push_back(TOKEN_ID::TOKEN_R_TRUE); //FLEX_STD cout << g_mapTokenName[TOKEN_ID::TOKEN_R_TRUE] << " << input: [" << YYText() << "]\n";
"false" 	g_lstTokenDetected.push_back(TOKEN_ID::TOKEN_R_FALSE); //FLEX_STD cout << g_mapTokenName[TOKEN_ID::TOKEN_R_FALSE] << " << input: [" << YYText() << "]\n";

{char}		{// g_lstTokenDetected.push_back(TOKEN_ID::TOKEN_EOF);//FLEX_STD cout << "char " << " << input: [" << YYText() << "]\n";
                        std::string sToken = yytext;
                        if (sToken == "("){
                                g_lstTokenDetected.push_back(TOKEN_ID::TOKEN_CHAR_open_1);
                        }
                        else if(sToken == ")"){
                                g_lstTokenDetected.push_back(TOKEN_ID::TOKEN_CHAR_close_1);
                        }
                        else if(sToken == "["){
                                g_lstTokenDetected.push_back(TOKEN_ID::TOKEN_CHAR_open_2);
                        }
                        else if(sToken == "]"){
                                g_lstTokenDetected.push_back(TOKEN_ID::TOKEN_CHAR_close_2);
                        }
                        else if(sToken == "{"){
                                g_lstTokenDetected.push_back(TOKEN_ID::TOKEN_CHAR_open_3);
                        }
                        else if(sToken == "}"){
                                g_lstTokenDetected.push_back(TOKEN_ID::TOKEN_CHAR_close_3);
                        }
                        else if(sToken == ";"){
                                g_lstTokenDetected.push_back(TOKEN_ID::TOKEN_CHAR_END_STATEMENT);
                        }
                        else if(sToken == "+"){
                                g_lstTokenDetected.push_back(TOKEN_ID::TOKEN_OP_PLUS);
                        }
                        else if(sToken == "-"){
                                g_lstTokenDetected.push_back(TOKEN_ID::TOKEN_OP_MINUS);
                        }
                        else if(sToken == "*"){
                                g_lstTokenDetected.push_back(TOKEN_ID::TOKEN_OP_MULTI);
                        }
                        else if(sToken == "/"){
                                g_lstTokenDetected.push_back(TOKEN_ID::TOKEN_OP_DIV);
                        }
                        else if(sToken == "="){
                                g_lstTokenDetected.push_back(TOKEN_ID::TOKEN_OP_EQUAL);
                        }
                        else if(sToken == "!"){
                                g_lstTokenDetected.push_back(TOKEN_ID::TOKEN_OP_NOT);
                        }
                        else if(sToken == "<"){
                                g_lstTokenDetected.push_back(TOKEN_ID::TOKEN_CONDITION_LT);
                        }
                        else if(sToken == ">"){
                                g_lstTokenDetected.push_back(TOKEN_ID::TOKEN_CONDITION_GT);
                        }
                }

{const_number}	g_lstTokenDetected.push_back(TOKEN_ID::NUMBER);//FLEX_STD cout << g_mapTokenName[TOKEN_ID::NUMBER] << " << input: [" << YYText() << "]\n";

\n		mylineno++;

{var_name}	g_lstTokenDetected.push_back(TOKEN_ID::ID_VAR);//FLEX_STD cout << g_mapTokenName[TOKEN_ID::ID_VAR] << " << input: [" << YYText() << "]\n";

{string}	g_lstTokenDetected.push_back(TOKEN_ID::STRING);//FLEX_STD cout << g_mapTokenName[TOKEN_ID::STRING]  << " << input: [" << YYText() << "]\n";

{condition} 	{//g_lstTokenDetected.push_back(TOKEN_ID::TOKEN_EOF);//FLEX_STD cout << "condition"  << " << input: [" << YYText() << "]\n";
                        std::string sToken = yytext;
                        if (sToken == ">="){
                                g_lstTokenDetected.push_back(TOKEN_ID::TOKEN_CONDITION_GTEQUAL);
                        }
                        if (sToken == "<="){
                                g_lstTokenDetected.push_back(TOKEN_ID::TOKEN_CONDITION_LTEQUAL);
                        }
                        if (sToken == "=="){
                                g_lstTokenDetected.push_back(TOKEN_ID::TOKEN_CONDITION_EQUALEQUAL);
                        }
                }
%%

#include <fstream>
#include <utility>
#include <string>
#include <iostream>

int main(int argc, char **argv)
{
        yyin = fopen("data/code.js", "r");
    //  FlexLexer* lexer = new yyFlexLexer;
    // while(lexer->yylex() != 0)
    // 	;
    // return 0;


//     std::ifstream f1("data/code.js", std::ios::in);
//     std::string str((std::istreambuf_iterator<char>(f1)),
// 		    std::istreambuf_iterator<char>());
//     std::cout << str << std::endl;


    // else
    // yyin = stdin;
    // lexer->
    yylex();

    for (int i = 0; i < g_lstTokenDetected.size(); i++)
    {
	TOKEN_ID id = g_lstTokenDetected[i];
	printf("\ntoken => %s", g_mapTokenName[id].c_str());
    }
    getchar();
}
// #endif