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
#include "CTokener.hpp"


class TokenManager;
class Tokener;
extern vector<string> splitString(string sInput, string sDelimiter);

class TokenManager
{
  public:
    TokenManager(string sPathFileGrammar);
	~TokenManager();

	static string m_sTokenNameEpsilon;
	static string m_sTokenNameDola;
	static string m_sTokenNameStart;


    // Kiem tra validate du lieu
    bool isParsed();

	// Lay doi tuong token 
	static Tokener* getToken(string sTokenName);

  private:
    // Doc file dinh nghia trang thai
    void loadFile(string sPathFile);
    
    // phan loai token ket thuc | chua ket thuc |
    bool classifyToken();

    // Khu de qui trai
    bool initStateInfer();

	// Cai dat danh sach doi tuong token 
	bool initTokenObject();

	// Cai dat tap first va follow 
	bool initFirstAndFollowSet();

	void showStateInfer();

	// sinh ten moi bang cach them ki tu '
	string generateNewName(string sName);

    bool isToken(string sName);
	bool addToken(string sName);
	bool addToken(vector<string> lstName);

	bool isTokenTerminal(string sName);
	bool genListTokenInfer(vector<vector<string>> lstStateInferName,
		vector<vector<Tokener*>> & lstStateInfer);

    bool m_bIsParsed;

    vector<string> m_lstTokenName;
	vector<string> m_lstStateInferDefine;

    vector<string> m_lstTokenTerminalName;
    vector<string> m_lstTokenNotTerminalName;
    
	unordered_map<string, vector<vector<string>>> m_mapStringInfer;
	static unordered_map<string, Tokener*> m_mapTokener;
};

#endif