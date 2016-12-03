#ifndef CDATAPARSER_H
#define CDATAPARSER_H

#include<stdio.h>
#include<iostream>
#include<regex>
#include<vector>
using namespace std;

#include "CTokenParser.hpp"

class DataParser
{
    public:
        DataParser(vector<TokenParser*>, string); 
        ~DataParser();
        
        bool tryParse();		// try format data input 
		bool tryParse(string sData);	// try format data input 
        void addToken(TokenParser*);
        void showAllLine();
        bool bIsParsed;
   
    private:
		int m_nPosInputPointer;	// point data parsed
		string m_sDataCode;		// data this file
		vector<TokenParser*> m_lstToken;	

		string getNextToken();	// get next token from m_nPosInputPointer

        void test ();
};

#endif