
#include "CTokener.hpp"

map<string, Tokener*> Tokener::mc_mapToken;
vector<Tokener*> Tokener::mc_lstTokenGettingFirst;

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

vector<Tokener *> Tokener::getFollowSet() 
{
	// Doc tap follow tu thuoc tinh cua class 
	return this->m_followSet;
}
// Tinh toan tap follow
bool Tokener::initFollowSet() {
	
	bool bRet = false;
	vector<Tokener *> followSetReturn;
	Tokener* tokenEpsilon = TokenManager::getToken(TokenManager::m_sTokenNameEpsilon);
	Tokener* tokenDola = TokenManager::getToken(TokenManager::m_sTokenNameDola);

	if (this->isTerminal())
		return bRet;

	// Luat 1 
	if (m_sTokenName == TokenManager::m_sTokenNameStart) {
		bRet = this->addFollowSet(tokenDola);
	}

	// Luat 2 - 3 - 4 
	for (auto stateInfer : this->m_lstStateInfer)
	{
		for (auto it = stateInfer.begin(); it != stateInfer.end(); it++) {
			Tokener* pTokenB = *it;
			if (pTokenB->isTerminal()) continue;
			if (it + 1 != stateInfer.end())
			{
				// Luat sinh co dang: alpha - B - beta 
				Tokener* pTokenBeta = *(it+1);
				vector <Tokener*> tmpFirstBeta = pTokenBeta->getFirstSet();
				auto itCheckEpsilon = find(tmpFirstBeta.begin(), tmpFirstBeta.end(), tokenEpsilon);
				if (itCheckEpsilon != tmpFirstBeta.end()) 
				{
					// Luat 4:  neu first beta co chua epsilon
					bool bRetTmp = pTokenB->addFollowSet(this->getFollowSet());
					bRet = bRet || bRetTmp;

					// xoa bo epsilon cho luat 2 
					tmpFirstBeta.erase(itCheckEpsilon);
				}
				// Luat 2 :  
				bool bRetTmp = pTokenB->addFollowSet(tmpFirstBeta);
				bRet = bRet || bRetTmp;

			}
			else {
				// Luat 3 - neu luat sinh co dang: alpha - B 
				bool bRetTmp = pTokenB->addFollowSet(this->getFollowSet());
				bRet = bRet || bRetTmp;
			}
		}
	}
	
	return bRet;
}

// Them token vao tap first
void Tokener::addFirstSet(vector<Tokener*> listToken)
{
	for (auto pToken : listToken) {
		this->addFirstSet(pToken);
	}
}

// Them token vao tap first
void Tokener::addFirstSet(Tokener* pToken)
{
	// Neu ds first set chua co pTokenB thi add them vao  
	if (isFirstSetContain(pToken) == false) {
		this->m_firstSet.push_back(pToken);
	}
}

// Kiem tra tap first co chua token hay khong
bool Tokener::isFirstSetContain(Tokener* pToken) {
	auto it = find(this->m_firstSet.begin(), this->m_firstSet.end(), pToken);
	return !(it == this->m_firstSet.end());
}

// Them token vao tap follow
bool Tokener::addFollowSet(vector<Tokener*> listToken)
{
	bool bRet = false;
	for (auto pToken : listToken) {
		bool tmpRet = this->addFollowSet(pToken);
		if (tmpRet == true)
			bRet = true;
	}
	return bRet;
}

// Them token vao tap follow
bool Tokener::addFollowSet(Tokener* pToken)
{
	// Neu ds first set chua co pTokenB thi add them vao  
	if (isFollowSetContain(pToken) == false) {
		this->m_followSet.push_back(pToken);
		return true;
	}
	return false;
}

// Kiem tra tap follow co chua token hay khong
bool Tokener::isFollowSetContain(Tokener* pToken) {
	auto it = find(this->m_followSet.begin(), this->m_followSet.end(), pToken);
	return !(it == this->m_followSet.end());
}