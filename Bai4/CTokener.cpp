
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
    m_mapToken[sTokenName] =  this;
}


Tokener::Tokener(string sTokenName)
{
    this->m_sTokenName = sTokenName;
    this->m_bIsTerminalToken = true;
    m_mapToken[sTokenName] = this;
}

Tokener::~Tokener()
{
    for (map<string, Tokener*>::iterator it = m_mapToken.begin();
         it != m_mapToken.end(); it++)
    {
        if (this->m_sTokenName == it->first){
            m_mapToken.erase(it);
            break;
        }
    }
}

bool Tokener::isTerminal(){
    return this->m_bIsTerminalToken;
}
