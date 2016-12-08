#ifndef CLINEPARSER_H
#define CLINEPARSER_H

#include <iostream>
#include <regex>
using namespace std;


class TokenParser
{
public:
	TokenParser(string line);
	string getName() {
		return m_sName;
	}
	string getSLexRegex() {
		return m_sLexRegex;
	}
	regex getLexRegex() {
		return m_lexRegex;
	}
	bool setLexRegex(string sLexRegex)
	{
		try {
			this->m_sLexRegex = sLexRegex;
			this->m_lexRegex = regex(sLexRegex);
		}
		catch (std::regex_error& ex) {
			cout << "Err exception parse regex: "<<sLexRegex <<": "<< ex.what()<<endl;
			return false;
		}
		catch (std::exception& ex) {
			cout << "Err exception parse regex: " <<  sLexRegex<<endl;
			return false;
		}
		return true;
	}
	void setCurrentInput(string sInputVal) {
		this->m_sValue = sInputVal;
	};
	string getCurrentInput() {
		return this->m_sValue;
	}
	bool isToken() {
		return m_bIsToken;
	}
	bool isParsed() {
		return m_bIsParsed;
	}

private:
	string m_dataLine;
	string m_sName;
	string m_sLexRegex;
	string m_sValue;
	bool m_bIsToken;
	bool m_bIsParsed;
	regex m_lexRegex;
};

#endif