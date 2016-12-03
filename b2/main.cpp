#include <stdio.h>
#include <regex>
#include <iostream>
#include <fstream>
#include <streambuf>
using namespace std;

#include "CDataParser.hpp"
#include "CTokenParser.hpp"

int main()
{
	// Du lieu cua file config va file code
	ifstream fileLexDef("data/lex-def");
	ifstream fileCodeJs("data/code.js");

	string line;
	string sDataCodeJs((istreambuf_iterator<char>(fileCodeJs)), std::istreambuf_iterator<char>());

	// Khoi tao doi tuong parser
	vector<TokenParser*> lstToken;
	DataParser* fileLexParser = new DataParser(lstToken, sDataCodeJs);
	
	
	// Doc + parse file regex config 
	while (getline(fileLexDef, line))
	{
		if(line.length() == 0) continue;
		TokenParser* tokenParser = new TokenParser(line);
		
		if(tokenParser->isParsed())
			fileLexParser->addToken(tokenParser);	
		else{
			delete(tokenParser);
			goto _EXIT_FUNCTION;
		}
		
	}

	fileLexParser->showAllLine();
    // regex integer("(\\+|-)?[[:digit:]]+");
	
	cout << sDataCodeJs;
	fileLexParser->tryParse();
    
	cin.get();

    // As long as the input is correct ask for another number
	
    // while (true)
    // {
	// 	cout << "Give me an integer!" << endl;
	// 	cin >> input;
	// 	if (!cin)
	// 		break;
	// 	//Exit when the user inputs q
	// 	if (input == "q")
	// 		break;
	// 	if (regex_match(input, integer))
	// 		cout << "integer" << endl;
	// 	else
	// 	{
	// 		cout << "Invalid input" << endl;
	// 	}
    // }

_EXIT_FUNCTION:
	delete(fileLexParser);
    return 0;
}