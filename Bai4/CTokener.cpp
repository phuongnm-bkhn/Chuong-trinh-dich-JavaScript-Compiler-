
#include "CTokener.hpp"

map<string, Tokener*> Tokener::m_mapToken;

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
	Tokener::m_mapToken[sTokenName] =  this;
}


Tokener::Tokener(string sTokenName)
{
    this->m_sTokenName = sTokenName;
    this->m_bIsTerminalToken = true;
	Tokener::m_mapToken[sTokenName] = this;
}

Tokener::~Tokener()
{
    for (map<string, Tokener*>::iterator it = Tokener::m_mapToken.begin();
         it != Tokener::m_mapToken.end(); it++)
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
