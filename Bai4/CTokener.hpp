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
	vector<vector<Tokener *>> getListTokenInfer() { return this->m_lstStateInfer; };
	
	// Tinh toan tap first
	vector<Tokener *> getFirstSet();

	// Tinh toan tap follow 
	vector<Tokener *> getFollowSet();
	bool initFollowSet();

	// Kiem tra tap first chua token
	bool isFirstSetContain(Tokener* pToken);
	
	// Kiem tra tap follow chua token
	bool isFollowSetContain(Tokener* pToken);

	// Lay ten 
	string getName() { return m_sTokenName; }

private:
	// Them token vao tap first
	void addFirstSet(Tokener* pToken);
	void addFirstSet(vector<Tokener*> pToken);

	// Them token vao tap first
	bool addFollowSet(Tokener* pToken);
	bool addFollowSet(vector<Tokener*> pToken);

	string m_sTokenName;
	bool m_bIsTerminalToken;
	vector<vector<Tokener *>> m_lstStateInfer;
	vector<Tokener *> m_firstSet;
	vector<Tokener *> m_followSet;

	static map<string, Tokener*> mc_mapToken;
	static vector<Tokener*> mc_lstTokenGettingFirst;

};

#endif