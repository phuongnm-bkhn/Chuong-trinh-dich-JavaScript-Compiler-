
#include "CTokener.hpp"

map<string, Tokener*> Tokener::mc_mapToken;
vector<Tokener*> Tokener::mc_lstTokenGettingFirst;
vector<Tokener*> Tokener::mc_lstTokenGettingFollow;

Tokener::Tokener(string sTokenName, vector<vector<Tokener *>> lstTokenInfer)
{
    this->m_sTokenName = sTokenName;
    this->m_lstStateInfer = lstTokenInfer;

    // neu khong suy dien ra token khac thi no la token terminal
    if(lstTokenInfer.size() == 0){
        this->m_bIsTerminalToken = true;
    }
    else {
        this->m_bIsTerminalToken = false;
    }
	Tokener::mc_mapToken[sTokenName] =  this;
}


Tokener::Tokener(string sTokenName, bool isTerminal)
{
    this->m_sTokenName = sTokenName;
    this->m_bIsTerminalToken = true;
	this->m_bIsTerminalToken = isTerminal;
	Tokener::mc_mapToken[sTokenName] = this;
}

Tokener::~Tokener()
{
    for (map<string, Tokener*>::iterator it = Tokener::mc_mapToken.begin();
         it != Tokener::mc_mapToken.end(); it++)
    {
        if (this->m_sTokenName == it->first){
            mc_mapToken.erase(it);
            break;
        }
    }
}

bool Tokener::isTerminal(){
    return this->m_bIsTerminalToken;
}

void Tokener::setListTokenInfer(vector<vector<Tokener *>> lstStateInfer) {
	this->m_lstStateInfer = lstStateInfer;
}

// Tinh toan tap first
vector<Tokener *> Tokener::getFirstSet() {
	
	// Tinh toan tap first
	if (this->m_firstSet.size() != 0)
		return this->m_firstSet;

	vector<Tokener *> firstSetReturn;
	Tokener* tokenEpsilon = TokenManager::getToken(TokenManager::m_sTokenNameEpsilon);

	// Kiem tra lap vo tan - xu ly loi truong hop luat suy dien loi
	auto itThisToken1 = find(Tokener::mc_lstTokenGettingFirst.begin(),
		Tokener::mc_lstTokenGettingFirst.end(), this);
	if (itThisToken1 != Tokener::mc_lstTokenGettingFirst.end())
		throw new runtime_error("Err: Get first set forever!");
	
	// Luu token hien tai vao danh sach
	Tokener::mc_lstTokenGettingFirst.push_back(this);

	if (this->isTerminal())
		// Neu token la terminal thi them no vao tap first
		this->addFirstSet(this);
	else {
		// Neu token la non terminal thi tinh toan tap first cua Y1 .. Yj 
		for (auto stateInfer : this->m_lstStateInfer) 
		{
			int nCountFirstTokenHasEpsilon = 0;
			for (auto pToken : stateInfer) 
			{
				// Neu tap first cuar token sinh ve phai hien tai chua epsilon 
				// thi tiep tuc add them tap first cua token tiep theo
				// Neu tap first cuar token sinh ve phai hien tai ko chua epsilon
				// thi khong can add them 
				vector<Tokener*> tmpFirst = pToken->getFirstSet();
				auto itCheckEpsilon = find(tmpFirst.begin(), tmpFirst.end(), tokenEpsilon);
				if (itCheckEpsilon == tmpFirst.end()) {
					// ko chua epsilon 
					this->addFirstSet(tmpFirst);
					break;
				}
				else {
					// chua epsilon
					tmpFirst.erase(itCheckEpsilon);
					this->addFirstSet(tmpFirst);
				}
				nCountFirstTokenHasEpsilon++;
			}

			// Neu tat ca deu chua epsilon thi them epsilon vao tap first 
			if (nCountFirstTokenHasEpsilon == stateInfer.size()) {
				this->addFirstSet(tokenEpsilon);
			}
		}
	}
	// Neu luat suy dien co chua epsilon thi them epsilon vao tap first
	for (auto lstToken : this->m_lstStateInfer) {
		auto it = find(lstToken.begin(), lstToken.end(), tokenEpsilon);
		if (it != lstToken.end()) {
			this->addFirstSet(tokenEpsilon);
		}
	}

	// xoa bo token hien tai trong ds 
	auto itThisToken2 = find(Tokener::mc_lstTokenGettingFirst.begin(),
		Tokener::mc_lstTokenGettingFirst.end(), this);
	if (itThisToken2 != Tokener::mc_lstTokenGettingFirst.end())
		Tokener::mc_lstTokenGettingFirst.erase(itThisToken2);

	return this->m_firstSet;
}

// Them token vao tap first
void Tokener::addFirstSet(Tokener* pToken)
{
	auto it = find(this->m_firstSet.begin(), this->m_firstSet.end(), pToken);
	if (it == this->m_firstSet.end()) {
		// Neu ds first set chua co pToken thi add them vao  
		this->m_firstSet.push_back(pToken);
	}
}

// Them token vao tap first
void Tokener::addFirstSet(vector<Tokener*> listToken)
{
	for (auto pToken : listToken) {
		this->addFirstSet(pToken);
	}
}
