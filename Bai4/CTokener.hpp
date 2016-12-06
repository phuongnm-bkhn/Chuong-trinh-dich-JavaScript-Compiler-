#ifndef C_TOKENER_H
#define C_TOKENER_H

#include <fstream>
#include <iostream>
#include <map>
#include <streambuf>
#include <vector>
using namespace std;
#include "libs.h"

class Tokener;

extern vector<string> splitString(string sInput, string sDelimiter);


class Tokener
{
  public:
    Tokener(string sTokenName, vector<vector<Tokener *>> m_lstTokenInfer);
    Tokener(string sTokenName);
    ~Tokener();

    bool isTerminal();

  private:
    string m_sTokenName;
    bool m_bIsTerminalToken;
    vector<vector<Tokener *>> m_lstTokenInfer;

	static map<string, Tokener*> m_mapToken;
};

#endif