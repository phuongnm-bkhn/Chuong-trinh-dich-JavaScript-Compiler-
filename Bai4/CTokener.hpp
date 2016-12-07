#ifndef C_TOKENER_H
#define C_TOKENER_H

#include <fstream>
#include <iostream>
#include <map>
#include <streambuf>
#include <vector>
using namespace std;
#include "libs.h"
#include "CTokenManager.hpp"

class Tokener;
class TokenManager;
extern vector<string> splitString(string sInput, string sDelimiter);


class Tokener
{
public:
	Tokener(string sTokenName, vector<vector<Tokener *>> lstStateInfer);
	Tokener(string sTokenName, bool isTerminal = false);

	~Tokener();

	bool isTerminal();
	void setListTokenInfer(vector<vector<Tokener *>> lstStateInfer);
	
	// Tinh toan tap first
	vector<Tokener *> getFirstSet();

	// Tinh toan tap follow 
	vector<Tokener *> getFollowSet();
	bool initFollowSet();

	// Lay ten 
	string getName() { return m_sTokenName; }

private:
	// Them token vao tap first
	void addFirstSet(Tokener* pToken);
	void addFirstSet(vector<Tokener*> pToken);
	bool isFirstSetContain(Tokener* pToken);

	// Them token vao tap first
	bool addFollowSet(Tokener* pToken);
	bool addFollowSet(vector<Tokener*> pToken);
	bool isFollowSetContain(Tokener* pToken);

	string m_sTokenName;
	bool m_bIsTerminalToken;
	vector<vector<Tokener *>> m_lstStateInfer;
	vector<Tokener *> m_firstSet;
	vector<Tokener *> m_followSet;

	static map<string, Tokener*> mc_mapToken;
	static vector<Tokener*> mc_lstTokenGettingFirst;

};

#endif