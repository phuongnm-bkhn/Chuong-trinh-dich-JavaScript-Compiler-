
#include "CTokener.hpp"

Tokener::Tokener(string sTokenName, vector<vector<Tokener *>> lstTokenInfer)
{
    this->m_sTokenName = sTokenName;
    this->m_lstTokenInfer = lstTokenInfer;

    // neu khong suy dien ra token khac thi no la token terminal
    if(lstTokenInfer.size() == 0){
        this->m_bIsTerminalToken = true;
    }
    else {
        this->m_bIsTerminalToken = false;
    }
    Tokener::m_mapToken.put(sTokenName, this);
}


Tokener::Tokener(string sTokenName)
{
    this->m_sTokenName = sTokenName;
    this->m_bIsTerminalToken = true;
    Tokener::m_mapToken.put(sTokenName, this);
}

Tokener::~Tokener()
{
    for (map<string, vector<Tokener *>>::iterator it = Tokener::m_lstToken.begin();
         it != Tokener::m_lstToken.end(); it++)
    {
        if (this == *it){
            Tokener::m_mapToken.erase(i);
            break;
        }
    }
}

bool Tokener::isTerminal(){
    return this->m_bIsTerminalToken;
}
