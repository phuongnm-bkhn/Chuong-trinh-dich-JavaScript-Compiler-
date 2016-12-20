#ifdef USE_FLEX
#else

#include "CDataParser.hpp"
#include "CLexer.h"
#include "CTokenParser.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
using namespace std;

//
map<TOKEN_ID, std::string> g_mapTokenName;

vector<string> getListToken1(string sPathFileCode)
{
    /*vector<TOKEN_ID> g_lstTokenDetected;
	g_lstTokenDetected.push_back(TOKEN_ID::ID_VAR);
	g_lstTokenDetected.push_back(TOKEN_ID::TOKEN_OP_PLUS);
	g_lstTokenDetected.push_back(TOKEN_ID::ID_VAR);
	g_lstTokenDetected.push_back(TOKEN_ID::TOKEN_OP_MULTI);
	g_lstTokenDetected.push_back(TOKEN_ID::ID_VAR);
*/

    vector<string> g_lstTokenDetected;
    /*g_lstTokenDetected.push_back("do");
	g_lstTokenDetected.push_back("+");
	g_lstTokenDetected.push_back("id");
	g_lstTokenDetected.push_back("*");
	g_lstTokenDetected.push_back("(");
	g_lstTokenDetected.push_back("id");
	g_lstTokenDetected.push_back("+");
	g_lstTokenDetected.push_back("id");
	g_lstTokenDetected.push_back(")");*/

    g_lstTokenDetected.push_back("do");
    g_lstTokenDetected.push_back("{");

    g_lstTokenDetected.push_back("var");
    g_lstTokenDetected.push_back("id");
    g_lstTokenDetected.push_back("=");
    g_lstTokenDetected.push_back("id");
    g_lstTokenDetected.push_back("+");
    g_lstTokenDetected.push_back("id");
    g_lstTokenDetected.push_back(";");

    g_lstTokenDetected.push_back("var");
    g_lstTokenDetected.push_back("id");
    g_lstTokenDetected.push_back("=");
    g_lstTokenDetected.push_back("id");
    g_lstTokenDetected.push_back("+");
    g_lstTokenDetected.push_back("id");
    g_lstTokenDetected.push_back("-");
    g_lstTokenDetected.push_back("num");
    g_lstTokenDetected.push_back(";");
    g_lstTokenDetected.push_back("}");
    g_lstTokenDetected.push_back("while");
    g_lstTokenDetected.push_back("(");
    g_lstTokenDetected.push_back("id");
    g_lstTokenDetected.push_back(">=");
    g_lstTokenDetected.push_back("num");
    g_lstTokenDetected.push_back(")");
    g_lstTokenDetected.push_back(";");

    return g_lstTokenDetected;
}

vector<string> getListToken(string sPathFileCode)
{
    // Du lieu cua file config va file code
    ifstream fileLexDef("data/lex-def");
    // ifstream fileCodeJs("data/code-utf8.js");
    ifstream fileCodeJs(sPathFileCode);

    string line;
    string sDataCodeJs((istreambuf_iterator<char>(fileCodeJs)), std::istreambuf_iterator<char>());

    // Khoi tao doi tuong parser
    vector<TokenParser *> lstToken;
    vector<string> lstTokenName;
    DataParser *fileLexParser = new DataParser(lstToken, sDataCodeJs);

    // Read + parse file regex config
    while (getline(fileLexDef, line))
    {
	if (line.length() == 0 || (line.size() >= 2 && line.substr(0, 2) == "//"))
	    continue;
	TokenParser *tokenParser = new TokenParser(line);

	if (tokenParser->isParsed())
	    fileLexParser->addToken(tokenParser);
	else
	{
	    delete (tokenParser);
	    goto _EXIT_FUNCTION;
	}
    }
    lstTokenName = fileLexParser->getListTokenIdInput();

_EXIT_FUNCTION:
    delete (fileLexParser);
    return lstTokenName;
}
#endif
