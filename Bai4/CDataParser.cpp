#include "CDataParser.hpp" 


DataParser::DataParser(vector<TokenParser*> lstToken, string sDataCode)
{
    this->m_lstToken = lstToken;
	this->m_nPosInputPointer = 0;
	this->m_sDataCode = sDataCode;
}

DataParser::~DataParser()
{
    for(int i = 0; i<this->m_lstToken.size(); i++)
    {
        if(this->m_lstToken.at(i) != NULL){
            free(this->m_lstToken.at(i));
            this->m_lstToken.at(i) = NULL;
        }
    }
}

void DataParser::addToken(TokenParser* lineParser){
    
	string sTermCheck = "{%s}";
	string sTokenNameCheck;
	string sLexRegexNew = lineParser->getSLexRegex();

	char sBuff[2048] = {0};

	// Duyet qua kiem tra tung token truoc do 
	// Thay the chuoi token truoc do neu su dung lai
	for (int i = 0; i < this->m_lstToken.size(); i++) 
	{
		// Doc ten token da ton tai
		sprintf(sBuff, sTermCheck.c_str(), this->m_lstToken.at(i)->getName().c_str());
		sBuff[2047] = 0;
		sTokenNameCheck = sBuff;

		// Thay the regex da ton tai tuong ung vao xau regex moi
		int nPos = 0;
		while (nPos != string::npos) {
			// Tim kiem vi tri ten tooken cu
			nPos = sLexRegexNew.find(sTokenNameCheck);
			if (nPos != string::npos) 
			{
				// thay the 
				sLexRegexNew.replace(nPos, sTokenNameCheck.length(),
					this->m_lstToken.at(i)->getSLexRegex());
			}
			else
				break;
		}
	}
	lineParser->setLexRegex(sLexRegexNew);
    this->m_lstToken.push_back(lineParser);
}

vector<string>DataParser::getListTokenIdInput() {
	vector<string> lstRet;
	this->m_nPosInputPointer = 0;
	while (m_nPosInputPointer < this->m_sDataCode.length())
	{
		TokenParser* pTokenFound  = this->getNextToken2();
		if (pTokenFound == NULL) {
			cout << "Err: not parse token from input! " << endl;
			lstRet.clear();
			goto _EXIT_FUNCTION;
		}
		else {
			if (pTokenFound->getName().find("%reserve_") == 0) {
				// cac token tring can lay chinh xac noi dung input 
				// do day la cac token tu khoa
				lstRet.push_back(pTokenFound->getCurrentInput());
			}
			else if (pTokenFound->getName().find("%skip_") == 0) {
				// do nothing - bo qua token nay
			}
			else {
				lstRet.push_back(pTokenFound->getName());
			}
		}
	}


_EXIT_FUNCTION:
	return lstRet;
}

void DataParser::showAllLine(){
	cout <<"List lex Token with regex: "<<endl;
    for (int i = 0 ; i< m_lstToken.size(); i++)
    {
        cout<< "name: "<<this->m_lstToken.at(i)->getName() << endl;
        cout<< "isToken: "<<this->m_lstToken.at(i)->isToken() << endl;
        cout<< "regex: "<<this->m_lstToken.at(i)->getSLexRegex() << endl;
		cout << endl;
    }
	cout << "======" << endl;
}

string DataParser::getNextToken()
{
	bool bRet = false;
	//regex cur("[a-z]([\\w\\d_])*");
	std::cmatch cm;    // same as std::match_results<const char*> cm;

	string sData = this->m_sDataCode.substr(m_nPosInputPointer).c_str();
	int nSizeToken = 0;
	string sTokenValue;
	string sTokenName;

	// using explicit flags:
	for (int i = 0; i < m_lstToken.size(); i++){
		
		if (m_lstToken.at(i)->isToken() == false)
			continue;
		
		std::regex_search(sData.c_str(), cm, m_lstToken.at(i)->getLexRegex(),
			regex_constants::match_continuous);
		
		if (cm.size()>0)
		{
			if (cm[0].length() > nSizeToken) {
				sTokenValue = cm[0];
				nSizeToken = sTokenValue.length();
				sTokenName = m_lstToken.at(i)->getName();
			}
		}
	}
	cout << "token: " << sTokenName << " val: " << sTokenValue << endl;

	m_nPosInputPointer += nSizeToken;
    return sTokenValue;
}

TokenParser* DataParser::getNextToken2()
{
	bool bRet = false;
	//regex cur("[a-z]([\\w\\d_])*");
	std::cmatch cm;    // same as std::match_results<const char*> cm;

	string sData = this->m_sDataCode.substr(m_nPosInputPointer).c_str();
	int nSizeToken = 0;
	TokenParser* tokenRet = NULL;
	string sTokenValue;
	string sTokenName;
	

	// using explicit flags:

	for (int i = 0; i < m_lstToken.size(); i++) {

		if (m_lstToken.at(i)->isToken() == false)
			continue;

		std::regex_search(sData.c_str(), cm, m_lstToken.at(i)->getLexRegex(),
			regex_constants::match_continuous);

		if (cm.size()>0)
		{
			if (cm[0].length() > nSizeToken) {
				sTokenValue = cm[0];
				nSizeToken = sTokenValue.length();
				sTokenName = m_lstToken.at(i)->getName();
				tokenRet = m_lstToken.at(i);
			}
		}
	}
	// cout << "token: " << sTokenName << " val: " << sTokenValue << endl;
	if (tokenRet != NULL) {
		tokenRet->setCurrentInput(sTokenValue);
	}
	else {
		int bug = 1;
	}

	m_nPosInputPointer += nSizeToken;
	return tokenRet;
}

bool DataParser::tryParse(string sData)
{
	while (m_nPosInputPointer < sData.length())
		cout << this->getNextToken() << endl;

	return true;
}

bool DataParser::tryParse()
{
	this->m_nPosInputPointer = 0;
	return this->tryParse(this->m_sDataCode);
}
