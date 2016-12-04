#include "CTokenManager.hpp"

TokenManager::TokenManager(string sPathFileGrammar)
{
    this->m_bIsParsed = false;
    this->m_sTokenNameEpsilon = "$";

    // Doc du lieu
    loadFile(sPathFileGrammar);

    // khu de qui trai

    vector<string> lstRet = this->m_lstTokenName;
    for (int i = 0; i < lstRet.size(); i++)
    {
        cout << lstRet.at(i) << endl;
    }

    lstRet = this->m_lstStateInfer;
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
    return false;
}

// phan loai token ket thuc | chua ket thuc |
bool TokenManager::classifyToken()
{
    string sName;
    for (string sStateInfer : this->m_lstStateInfer)
    {
        // sName = 
        for (string sTokenName : this->m_lstTokenName)
        {
        }
    }
}

// Validate du lieu
bool TokenManager::isParsed()
{
    return this->m_bIsParsed;
}