#ifndef C_TOKEN_MANAGER_H
#define C_TOKEN_MANAGER_H

#include <fstream>
#include <iostream>
#include <map>
#include <unordered_map>
#include <streambuf>
#include <vector>
#include <string>
using namespace std;
#include "libs.h"

extern vector<string> splitString(string sInput, string sDelimiter);

class TokenManager
{
  public:
    TokenManager(string sPathFileGrammar);

   
    // Kiem tra validate du lieu
    bool isParsed();

  private:
    // Doc file dinh nghia trang thai
    void loadFile(string sPathFile);
    
    // phan loai token ket thuc | chua ket thuc |
    bool classifyToken();

    // Khu de qui trai
    bool initStateInfer();



    bool isToken(string sName);
	bool isTokenTerminal(string sName);

    bool m_bIsParsed;
    string m_sTokenNameEpsilon;
    vector<string> m_lstTokenName;
    vector<string> m_lstTokenTerminalName;
    vector<string> m_lstTokenNotTerminalName;
    vector<string> m_lstStateInferDefine;

};

#endif