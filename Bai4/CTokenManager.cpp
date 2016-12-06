#include "CTokenManager.hpp"

TokenManager::TokenManager(string sPathFileGrammar)
{
    this->m_bIsParsed = false;
    this->m_sTokenNameEpsilon = "~";

    // read data and save to m_lstTokenName, m_lstStateInferDefine
    this->loadFile(sPathFileGrammar);

    // classify symbol terminal and not terminal
    // and save to m_lstTokenTerminalName, m_lstTokenNotTerminalName
    this-> classifyToken(); 

    // remove left recursion and save token
     this->initStateInfer();

    vector<string> lstRet = this->m_lstTokenTerminalName;
    for (int i = 0; i < lstRet.size(); i++)
    {
        cout << lstRet.at(i) << endl;
    }

    lstRet = this->m_lstTokenNotTerminalName;
    for (int i = 0; i < lstRet.size(); i++)
    {
        cout << lstRet.at(i) << endl;
    }
}

// Doc du lieu file
void TokenManager::loadFile(string sPathFile)
{
    map<string, vector<string>> mapRet;

    // Du lieu cua file dinh nghia cu phap
    ifstream fileStateDef(sPathFile);
    string line;
    vector<string> vtTmp;

    // Read + parse file grammar
    while (getline(fileStateDef, line))
    {
        if (line.length() == 0 ||
            (line.length() >= 2 && line.at(0) == '/' && line.at(1) == '/'))
            continue;
        if (line.find("%tokens:") == 0)
        {
            line = line.substr((string("%tokens:")).size());
            vtTmp = splitString(line, ",");
            this->m_lstTokenName.insert(this->m_lstTokenName.end(), vtTmp.begin(), vtTmp.end());
        }
        else
        {
            this->m_lstStateInferDefine.push_back(line);
        }
    }
}

// Khu de qui trai
bool TokenManager::initStateInfer()
{
	bool bRet = false;
	string sStateInfer, sNameTokenLeft;
	vector<string> lstInferRight;
	unordered_map<string, vector<vector<string>>> mapInfer;
	unordered_map<string, vector<vector<string>>> mapInferRemovedLeftRecursion;


	// Init state infer 
	// --
	// Cai dat cac luat sinh tu file dinh nghia
	for (vector<string>::iterator it = m_lstStateInferDefine.begin();
	it != this->m_lstStateInferDefine.end(); it++)
	{
		// lay trang thai suy dien vd string : A -> B , C | D
		sStateInfer = *it;
		vector<vector<string>> lstNameTokenInferRight;
		vector<string> lstTmp = splitString(sStateInfer, "->");
		if (lstTmp.size() != 2)
			goto _EXIT_FUNCTION;

		// ten token nguon  
		sNameTokenLeft = lstTmp.at(0);
		if (isToken(sNameTokenLeft) == false) {
			cout << "Err: this is not token: " << sNameTokenLeft << endl;
			goto _EXIT_FUNCTION;
		}

		lstInferRight = splitString(lstTmp.at(1), "|");	// ds trang thai dich 

		// Duyet qua tung string trang thai dich ben phai 
		for (string sToken : lstInferRight)
		{
			lstTmp = splitString(sToken, " ");
		
			// Kiem tra ten token ben phai luat sinh
			for (string sTokenNameCheck : lstTmp) {
				if (isToken(sTokenNameCheck) == false) {
					cout << "Err: this is not token: " << sTokenNameCheck << endl;
					goto _EXIT_FUNCTION;
				}
			}
			lstNameTokenInferRight.push_back(lstTmp);
		}
		if (mapInfer.find(sNameTokenLeft) ==
			mapInfer.end()) {
			// Khoi tao ds sinh 
			mapInfer[sNameTokenLeft] = lstNameTokenInferRight;
		}
		else {
			// Cap nhat them vao mang ds sinh
			vector<vector<string>> lstStateBefore =
				mapInfer[sNameTokenLeft];

			lstStateBefore.insert(lstStateBefore.end(),
				lstNameTokenInferRight.begin(), lstNameTokenInferRight.end());

			mapInfer[sNameTokenLeft] = lstStateBefore;
		}
	}

	// Reformat state infer 
	// -- 
	// Cau truc lai ds luat sinh 
	// - Xoa de qui gian tien 
	// - Xoa de qui trai 
	for (auto it = mapInfer.begin(); it != mapInfer.end(); ++it) 
	{ 
		// Xu ly tung luat suy dien -> luu vao mang mapInferRemovedLeftRecursion
		string sTokenName = it->first;	// ve trai luat sinh
		vector<vector<string>> lstInferRightDefine = it->second;	// ve phai luat sinh 
		vector<vector<string>> lstInferRightChange;	// ve phai luat sinh 
		bool bRecheck;
		do {
			bRecheck = false;
			for (auto lstTokenNameRightDefine = lstInferRightDefine.begin();
			lstTokenNameRightDefine != lstInferRightDefine.end(); ++lstTokenNameRightDefine)
			{
				// lay trang thai suy dien ben phai hien tai cua sTokenName
				vector<string> lstTokenNameRightChange = *lstTokenNameRightDefine;	// ve phai luat sinh 

				if (lstTokenNameRightChange.size() == 0) {
					cout << "Err: List infer size = 0 of " << sTokenName << endl;
					goto _EXIT_FUNCTION;
				}

				// lay ten token dau tien va thay the boi tap token da duoc dinh nghia truoc 
				string sNameTokenFirstRight = lstTokenNameRightDefine->at(0);
				auto it4 = mapInferRemovedLeftRecursion.find(sNameTokenFirstRight);

				// Neu token id dau tien da duoc dinh nghia trong luat sinh truoc do 
				// -> thay the token dau tien nay boi tap cac trang thai sinh ve phai 
				if (it4 != mapInferRemovedLeftRecursion.end()) {
					vector<vector<string>> lstTokenStateInferDefined = it4->second;
					for (vector<string> lstTokenNameInferDefined : lstTokenStateInferDefined) {
						lstTokenNameRightChange = lstTokenNameInferDefined;
						lstTokenNameRightChange.insert(lstTokenNameRightChange.end(),
							lstTokenNameRightDefine->begin() + 1, lstTokenNameRightDefine->end());
						lstInferRightChange.push_back(lstTokenNameRightChange);
					}
					bRecheck = true;
				}
				else {
					// TH bt - cac luat sinh tach biet nhau 
					lstInferRightChange.push_back(*lstTokenNameRightDefine);
				}
			}
			lstInferRightDefine = lstInferRightChange;
			lstInferRightChange.clear();

		} while (bRecheck);
		mapInferRemovedLeftRecursion[sTokenName] = lstInferRightDefine;
	}



_EXIT_FUNCTION:
	return bRet;
}

// phan loai token ket thuc | chua ket thuc |
bool TokenManager::classifyToken()
{
    string sName;
    bool bRet = true;
    this->m_lstTokenTerminalName.insert(this->m_lstTokenTerminalName.end(),
                                        this->m_lstTokenName.begin(), this->m_lstTokenName.end());

    // Check each state infer of grammar
    for (string sStateInfer : this->m_lstStateInferDefine)
    {
        vector<string> lstTmp = splitString(sStateInfer, "->");
        bool bCheck = false;
        if (lstTmp.size() != 2)
        {
            bRet = false;
            break; // false struct
        }
        else
            sName = lstTmp.at(0);

        // check in list symbol terminal
        for (vector<string>::iterator it = this->m_lstTokenTerminalName.begin();
             it != this->m_lstTokenTerminalName.end(); it++)
        {
            string sTokenName = *it;
            if (sName == sTokenName)
            {
                this->m_lstTokenTerminalName.erase(it);
                bCheck = true;
                break;
            }
        }

        // check in list symbol not terminal
        if (bCheck == true)
        {
            this->m_lstTokenNotTerminalName.push_back(sName);
        }
        else
        {
            int i = 0;
            for (i = 0; i < this->m_lstTokenNotTerminalName.size(); i++)
            {
                if (this->m_lstTokenNotTerminalName.at(i) == sName)
                    break;
            }
            if (i == this->m_lstTokenNotTerminalName.size())
            {
                bRet = false;
                break;
            }
        }
    }
    return bRet;
}

// Validate du lieu
bool TokenManager::isParsed()
{
    return this->m_bIsParsed;
}

// Kiem tra token 
bool TokenManager::isToken(string sName)
{
    bool bCheck = false;
    for (vector<string>::iterator it = this->m_lstTokenName.begin();
         it != this->m_lstTokenName.end(); it++)
    {
        string sTokenName = *it;
        if (sName == sTokenName)
        {
            bCheck = true;
            break;
        }
    }
    return bCheck;
}

// Kiem tra token terminal 
bool TokenManager::isTokenTerminal(string sName)
{
	bool bCheck = false;
	for (vector<string>::iterator it = this->m_lstTokenTerminalName.begin();
	it != this->m_lstTokenTerminalName.end(); it++)
	{
		string sTokenName = *it;
		if (sName == sTokenName)
		{
			bCheck = true;
			break;
		}
	}
	return bCheck;
}