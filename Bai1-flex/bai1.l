%option C++ noyywrap

%{
int mylineno = 0;
using namespace std;
#include<map>

enum TOKEN_ID {
        // Kieu du lieu co ban 
        ID_VAR = 0,
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
        TOKEN_OP_PLUS,
        TOKEN_OP_PLUSPLUS,
        TOKEN_OP_MINUS,
        TOKEN_OP_MINUSMINUS,
        TOKEN_OP_DIV,
        TOKEN_OP_MULTI,
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
        
        return mapTokenName;
}

std::map<TOKEN_ID, std::string> g_mapTokenName = InitTokenName();
%}

string	((\"[^\n"]+\")|(\'[^\n']+\'))

ws	[ \t]+

alpha	[A-Za-z]
dig	[0-9]
var_name	({alpha}|\$)({alpha}|{dig}|\_|\.|\-|\/|\$)*

num1	[-+]?{dig}+\.?([eE][-+]?{dig}+)?
num2	[-+]?{dig}*\.{dig}+([eE][-+]?{dig}+)?
const_number	{num1}|{num2}

condition (==|>=|<=)
char    [(){}\[\];\+\-\/*=!?:><]

%%

{ws}	/* skip blanks and tabs */

"/*"		{
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

"do" 		FLEX_STD cout << g_mapTokenName[TOKEN_ID::TOKEN_R_DO] << " << input: [" <<YYText() << "]\n";
"while"  	FLEX_STD cout << g_mapTokenName[TOKEN_ID::TOKEN_R_WHILE] << " << input: [" << YYText() << "]\n";
"if" 		FLEX_STD cout << g_mapTokenName[TOKEN_ID::TOKEN_R_IF] << " << input: [" << YYText() << "]\n";
"then"  	FLEX_STD cout << g_mapTokenName[TOKEN_ID::TOKEN_R_THEN] << " << input: [" << YYText() << "]\n";
"else" 		FLEX_STD cout << g_mapTokenName[TOKEN_ID::TOKEN_R_ELSE] << " << input: [" << YYText() << "]\n";
"var" 		FLEX_STD cout << g_mapTokenName[TOKEN_ID::TOKEN_R_VAR] << " << input: [" << YYText() << "]\n";
"function"	FLEX_STD cout << g_mapTokenName[TOKEN_ID::TOKEN_R_FUNCTON] << " << input: [" << YYText() << "]\n";
"return" 	FLEX_STD cout << g_mapTokenName[TOKEN_ID::TOKEN_R_RETURN] << " << input: [" << YYText() << "]\n";
"true" 		FLEX_STD cout << g_mapTokenName[TOKEN_ID::TOKEN_R_TRUE] << " << input: [" << YYText() << "]\n";
"false" 	FLEX_STD cout << g_mapTokenName[TOKEN_ID::TOKEN_R_FALSE] << " << input: [" << YYText() << "]\n";

{char}		FLEX_STD cout << "char " << " << input: [" << YYText() << "]\n";


{const_number}	FLEX_STD cout << g_mapTokenName[TOKEN_ID::NUMBER] << " << input: [" << YYText() << "]\n";

\n		mylineno++;

{var_name}	FLEX_STD cout << g_mapTokenName[TOKEN_ID::ID_VAR] << " << input: [" << YYText() << "]\n";

{string}	FLEX_STD cout << g_mapTokenName[TOKEN_ID::STRING]  << " << input: [" << YYText() << "]\n";

{condition} 	FLEX_STD cout << "condition"  << " << input: [" << YYText() << "]\n";

%%

int main( int /* argc */, char** /* argv */ )
	{
	FlexLexer* lexer = new yyFlexLexer;
	while(lexer->yylex() != 0)
		;
	return 0;
	}
