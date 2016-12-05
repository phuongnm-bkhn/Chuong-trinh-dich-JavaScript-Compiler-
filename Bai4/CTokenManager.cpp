#include "CTokenManager.hpp"

TokenManager::TokenManager(string sPathFileGrammar)
{
    this->m_bIsParsed = false;
    this->m_sTokenNameEpsilon = "$";

    // read data and save to m_lstTokenName, m_lstStateInfer
    this->loadFile(sPathFileGrammar);

    // classify symbol terminal and not terminal
    // and save to m_lstTokenTerminalName, m_lstTokenNotTerminalName
    this-> classifyToken(); 

    // remove left recursion and save token
    // this->removeLeftRecursion();

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
            this->m_lstStateInfer.push_back(line);
        }
    }
}

// Khu de qui trai
bool TokenManager::removeLeftRecursion()
{
    bool bRet = false;
    string sStateInfer, sNameTokenLeft;
    vector<string> lstInferRight;
    vector<vector<string>> lstNameTokenInferRight;
    for (vector<string>::iterator it; it != this->m_lstStateInfer.end(); it++)
    {
        sStateInfer = *it;
        vector<string> lstTmp = splitString(sStateInfer, "->");
        if (lstTmp.size() != 2)
            break;
        sNameTokenLeft = lstTmp.at(0);
        lstInferRight = splitString(lstTmp.at(1), "|");
        // for 
    }
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
    for (string sStateInfer : this->m_lstStateInfer)
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

// Validate du lieu
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