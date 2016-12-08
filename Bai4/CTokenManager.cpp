#include "CTokenManager.hpp"

string TokenManager::m_sTokenNameEpsilon = "~";
string TokenManager::m_sTokenNameDola = "$";
string TokenManager::m_sTokenNameStart;

unordered_map<string, Tokener*> TokenManager::m_mapTokener;

TokenManager::TokenManager(string sPathFileGrammar)
{
    this->m_bIsParsed = false;

    // read data and save to m_lstTokenName, m_lstStateInferDefine
    this->loadFile(sPathFileGrammar);

    // classify symbol terminal and not terminal
    // and save to m_lstTokenTerminalName, m_lstTokenNotTerminalName
    this-> classifyToken(); 

    // remove left recursion and save token
     this->initStateInfer();
	 
	 // save state infer to object c++
	 this->initTokenObject();

	 // init follow and follow
	 this->initFirstAndFollowSet();

	 // init grammar table
	 this->initGrammarTable();
	 
	 this->showStateInfer();

    /*vector<string> lstRet = this->m_lstTokenTerminalName;
    for (int idState = 0; idState < lstRet.size(); idState++)
    {
        cout << lstRet.at(idState) << endl;
    }

    lstRet = this->m_lstTokenNotTerminalName;
    for (int idState = 0; idState < lstRet.size(); idState++)
    {
        cout << lstRet.at(idState) << endl;
    }*/
}

TokenManager::~TokenManager() {
	for (unordered_map<string, Tokener*>::iterator pTokener = TokenManager::m_mapTokener.begin(); 
	pTokener != TokenManager::m_mapTokener.end() ; pTokener++) {
		delete (pTokener->second);
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

            this->addToken(vtTmp);
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
	unordered_map<string, vector<vector<string>>> mapInferRemovedIndirectRecursion;


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
		if (mapInfer.find(sNameTokenLeft) == mapInfer.end()) {
			// Khoi tao ds sinh 
			mapInfer[sNameTokenLeft] = lstNameTokenInferRight;
		}
		else {
			// Cap nhat them vao mang ds sinh
			vector<vector<string>> lstStateBefore =	mapInfer[sNameTokenLeft];

			lstStateBefore.insert(lstStateBefore.end(),
				lstNameTokenInferRight.begin(), lstNameTokenInferRight.end());

			mapInfer[sNameTokenLeft] = lstStateBefore;
		}
	}

	// Reformat state infer 
	// remove Indirect left recusive 
	// -- 
	// Cau truc lai ds luat sinh 
	// - Xoa de qui gian tien 
	// - Xoa de qui trai 
	for (auto it = mapInfer.begin(); it != mapInfer.end(); ++it) 
	{ 
		// Xu ly tung luat suy dien -> luu vao mang mapInferRemovedIndirectRecursion
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
				if (lstTokenNameRightChange.size() == 1 && lstTokenNameRightChange.at(0) == sTokenName) {
					cout << "Err: Bad infer me->me " << sTokenName << endl;
					goto _EXIT_FUNCTION;
				}

				// lay ten token dau tien va thay the boi tap token da duoc dinh nghia truoc 
				string sNameTokenFirstRight = lstTokenNameRightDefine->at(0);
				auto it4 = mapInferRemovedIndirectRecursion.find(sNameTokenFirstRight);

				// Neu token id dau tien da duoc dinh nghia trong luat sinh truoc do 
				// -> thay the token dau tien nay boi tap cac trang thai sinh ve phai 
				if (it4 != mapInferRemovedIndirectRecursion.end()) {
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


		// Remove left recusive 
		// -----
		// Cau truc lai ds luat sinh 
		// - Xoa de qui trai 
		lstInferRightChange.clear();
		string sTokenNameNew = this->generateNewName(sTokenName);
		vector<vector<string>> lstInferRightForCurentTokenName;	// ve phai luat sinh 
		vector<vector<string>> lstInferRightForNewTokenName;	// ve phai luat sinh 
		for (auto lstTokenNameRight : lstInferRightDefine) {
			vector<string> lstTokenNameNew;
			if (lstTokenNameRight.at(0) != sTokenName && this->isTokenTerminal(sTokenName) == false) 
			{
				// sinh ds token moi cho token hien tai: sTokenName
				if (lstTokenNameRight.at(0) == this->m_sTokenNameEpsilon) {
					// neu la epsilon -> them token trung gian moi
					lstTokenNameNew.push_back(sTokenNameNew);
				}
				else {
					// Them vao tat ca ds token cu + name token trung gian moi 
					lstTokenNameNew.insert(lstTokenNameNew.begin(), 
						lstTokenNameRight.begin(), lstTokenNameRight.end());
					lstTokenNameNew.push_back(sTokenNameNew);
				}
				lstInferRightForCurentTokenName.push_back(lstTokenNameNew);
			}
			else {
				// sinh ds token moi cho token gian tiep cua token hien tai: sTokenName
				lstTokenNameNew.insert(lstTokenNameNew.begin(),
					lstTokenNameRight.begin() + 1, lstTokenNameRight.end());
				lstTokenNameNew.push_back(sTokenNameNew);
				lstInferRightForNewTokenName.push_back(lstTokenNameNew);
			}
		}

		if (lstInferRightForNewTokenName.size() > 0) {
			// Co de qui trai 
			// them ds trang thai epsilon vao cuoi bo sinh cua token trung gian 
			vector<string> tmpEpsilon;
			tmpEpsilon.push_back(this->m_sTokenNameEpsilon);
			lstInferRightForNewTokenName.push_back(tmpEpsilon);

			this->m_lstTokenName.push_back(sTokenNameNew);
			this->m_lstTokenNotTerminalName.push_back(sTokenNameNew);
			mapInferRemovedIndirectRecursion[sTokenName] = lstInferRightForCurentTokenName;
			mapInferRemovedIndirectRecursion[sTokenNameNew] = lstInferRightForNewTokenName;
		}
		else {
			// Khong ton tai de qui trai 
			mapInferRemovedIndirectRecursion[sTokenName] = lstInferRightDefine;
		}
	}

	// Luu vao thuoc tinh cua doi tuong 
	this->m_mapStringInfer = mapInferRemovedIndirectRecursion;

	bRet = true;
_EXIT_FUNCTION:
	return bRet;
}

// Cai dat danh sach doi tuong token 
bool TokenManager::initTokenObject() 
{
	bool bRet = false;
	// Cai dat du lieu cho cac toi tuong token 
	for (auto sNameToken : this->m_lstTokenName) {
		TokenManager::m_mapTokener[sNameToken] = 
			new Tokener(sNameToken, isTokenTerminal(sNameToken));
	}

	if (m_mapStringInfer.size() > 0) 
	{
		// Cai dat ten cho ki tu bat dau = ki tu dau ben ve phai luat sinh
		for (auto& tokenInfo : m_mapStringInfer) {
			TokenManager::m_sTokenNameStart = tokenInfo.first;
			break;
		}
	}

	// Them cac con tro lien ket giua cac doi tuong tuong tu nhu trong luat sinh 
	for (auto& tokenInfo : m_mapStringInfer) {
		auto it = TokenManager::m_mapTokener.find(tokenInfo.first);
		if (it != m_mapTokener.end())
		{
			Tokener* pTokener = it->second;
			// Lay danh sach token ifer 
			vector<vector<Tokener*>> lstTokenInfer;
			if (genListTokenInfer(tokenInfo.second, lstTokenInfer) == false) {
				goto _EXIT_FUNCTION;
			}
			pTokener->setListTokenInfer(lstTokenInfer);
		}
	}
	bRet = true;

_EXIT_FUNCTION:
	return bRet;
}

// Cai dat tap follow va follow 
bool TokenManager::initFirstAndFollowSet() {
	
	// init follow
	for (auto& tokenInfo : this->m_mapTokener)
	{
		Tokener* pToken = tokenInfo.second;
		pToken->getFirstSet();
	}

	// init follow
	bool bCheckFollowChange = false;
	do {
		bCheckFollowChange = false;
		for (auto& tokenInfo : this->m_mapStringInfer)
		{
			Tokener* pToken = getToken(tokenInfo.first);
			bool bTmpCheck = pToken->initFollowSet();
			bCheckFollowChange = bCheckFollowChange || bTmpCheck;
		}
	} while (bCheckFollowChange);

	return true;
}

// Cai dat bang grammar 
bool TokenManager::initGrammarTable() 
{
	bool bRet = true;

	// Duyet qua tung luat sinh -> luu vao bang gramar table
	for (auto& stateInferDefine : this->m_mapStringInfer) 
	{
		// lay token nguon 
		Tokener* pTokenNotTerminal = getToken(stateInferDefine.first);
		
		// 1 token nguon co the co nhieu luat sinh: 
		// duyet qua tung luat sinh phai cua token nguon
		vector<vector<Tokener*>>& lstStateInfer = pTokenNotTerminal->getListTokenInfer();
		for (int idState = 0; idState < lstStateInfer.size(); idState++) 
		{
			vector<Tokener*>& stateInfer = lstStateInfer.at(idState);
			if (stateInfer.size() == 0) goto _EXIT_FUNCTION;
			
			// Them vao cac terminal symbol in first (alpha)
			vector<Tokener*>& lstTerminalOfFirst = stateInfer.at(0)->getFirstSet();
			for (auto pTokenTerminal : lstTerminalOfFirst) {
				if (pTokenTerminal->getName() == TokenManager::m_sTokenNameEpsilon) continue;	// bo qua epsilon
				bool tmpRet = this->addStateInferToGrammarTable(pTokenNotTerminal, pTokenTerminal, idState);
				bRet = tmpRet && bRet;
			}

			// Them vao cac terminal symbol in follow (A)
			if (stateInfer.at(0)->isFirstSetContain(getToken(TokenManager::m_sTokenNameEpsilon)))
			{
				vector<Tokener*>& lstTerminalOfFollow = pTokenNotTerminal->getFollowSet();
				for (auto pTokenTerminal : lstTerminalOfFollow) {
					bool tmpRet = this->addStateInferToGrammarTable(pTokenNotTerminal, pTokenTerminal, idState);
					bRet = tmpRet && bRet;
				}
			}
		}
	}

_EXIT_FUNCTION:
	return bRet;
}

// Hien thi ds trang thai suy dien cua he thong 
void TokenManager::showStateInfer() {

	cout << "- List state infer before: " << endl;
	for (auto state : m_lstStateInferDefine) {
		cout << state << endl;
	}
	cout << endl;

	cout << "- List token terminal: " << endl;
	for (auto state : m_lstTokenTerminalName) {
		cout << state << " ";
	}
	cout << endl;
	cout << endl;

	cout << "- List token non terminal: " << endl;
	for (auto state : m_lstTokenNotTerminalName) {
		cout << state << " ";
	}
	cout << endl;
	cout << endl;

	cout << "- List state infer after : " << endl;
	int i = 0;
	for (auto element : this->m_mapStringInfer) {
		int j = 0;
		string sNameToken = element.first;
		vector<vector<string>> lstTokenInfer = element.second;
		cout << sNameToken << "->";
		for (auto lstToken : lstTokenInfer) {
			int k = 0;
			for (auto sToken : lstToken) {
				cout << sToken;
				if (++k != lstToken.size())
					cout << " ";
			}
			if (++j != lstTokenInfer.size())
				cout << "|";
		}

		if (++i != this->m_mapStringInfer.size())
			cout << endl;
	}
	cout << endl;
	cout << endl;

	cout << "- List first set : " << endl;
	for (auto sTokenName : this->m_lstTokenName) {
		Tokener* pToken = TokenManager::getToken(sTokenName);
		vector<Tokener*> first = pToken->getFirstSet();
		cout << "First (" + sTokenName + ") = ";
		for (auto pTokenInFirst : first) {
			cout << pTokenInFirst->getName() << " ";
		}
		cout << endl;
	}
	cout << endl;
	cout << endl;

	cout << "- List follow set : " << endl;
	for (auto sTokenName : this->m_lstTokenName) {
		Tokener* pToken = TokenManager::getToken(sTokenName);
		if (pToken->isTerminal()) continue;
		vector<Tokener*> follow = pToken->getFollowSet();
		cout << "Follow (" + sTokenName + ") = ";
		for (auto pTokenInFollow : follow) {
			cout << pTokenInFollow->getName() << " ";
		}
		cout << endl;
	}
	cout << endl;
	cout << endl;

	cout << "- Gramar table : " << endl;
	unordered_map<string, int> mapShow;
	for (auto& row : m_mapGrammarTable) {
		for (auto& col : row.second) {
			if (mapShow.find(col.first->getName()) == mapShow.end()) {
				mapShow[col.first->getName()] = mapShow.size();
			}
		}
	}
	printf(" [%5s] ", "");
	for (auto col : mapShow) {
		printf(" [%5s] ", col.first.c_str());
	}
	cout << endl;
	for (int i = 0; i < mapShow.size() + 1; i++) {
		printf("=[%5s]=", "=====");
	}
	cout << endl;
	for (auto row : m_mapGrammarTable) {
		printf(" [%5s] ", row.first->getName().c_str());
		for (auto col : mapShow) {
			if (row.second.find(getToken(col.first)) != row.second.end()) {
				printf(" [%5d] ", row.second[getToken(col.first)]);
			}
			else
			{
				printf(" [%5s] ", "");
			}
		}
		cout << endl;
	}

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
Tokener* TokenManager::getToken(string sTokenName) {
	
	auto it = TokenManager::m_mapTokener.find(sTokenName);
	if (it != TokenManager::m_mapTokener.end())
		return it->second;
	else 
		return NULL;
}

// Hien thi ds trang thai suy dien cua he thong 
bool TokenManager::tryParse(vector<string> lstTokenInputCodeJs) 
{
	bool bRet = false;
	bool bContinueParse = true;
	int ip = 0;
	Tokener* pTokenDola = getToken(TokenManager::m_sTokenNameDola);
	Tokener* pTokenEpsilon = getToken(TokenManager::m_sTokenNameEpsilon);

	// stack processing 
	vector<Tokener*> lstTokenStack;
	lstTokenStack.push_back(pTokenDola);
	lstTokenStack.push_back(getToken(TokenManager::m_sTokenNameStart));

	// Add $ to list token input if input not contain $
	if (lstTokenInputCodeJs.at(lstTokenInputCodeJs.size() - 1) != TokenManager::m_sTokenNameDola) 
		lstTokenInputCodeJs.push_back(TokenManager::m_sTokenNameDola);
	
	// input processing 
	vector<Tokener*> lstTokenInput;
	for (auto sTokenName : lstTokenInputCodeJs) {
		Tokener* pToken = getToken(sTokenName);
		if (pToken == NULL) {
			cout << "Err: preprocess: token name = '" << sTokenName << "' not found!" << endl;
			goto _EXIT_FUNCTION;
		}
		lstTokenInput.push_back(pToken);
	}

	do {
		// Loi logic
		if (lstTokenInput.size() == 0 || lstTokenStack.size() == 0 || ip >= lstTokenInput.size())
			break;

		Tokener* pTokenHeadStack = *(lstTokenStack.end() - 1);
		Tokener* pTokenInput = lstTokenInput.at(ip);
		
		// TH1 X = a  = $ 
		if (pTokenHeadStack == pTokenInput && pTokenInput == pTokenDola) {
			bRet = true;
			bContinueParse = false;
			goto _EXIT_FUNCTION;
		}

		// TH2 X = a  != $ 
		if (pTokenHeadStack == pTokenInput && pTokenInput != pTokenDola) {
			lstTokenStack.pop_back();
			ip++;
		}

		// TH3 X is not terminal 
		if (pTokenHeadStack->isTerminal() == false) {
			if (isContainInGrammarTable(pTokenHeadStack, pTokenInput)) {
				vector<Tokener*> lstTokenInfer = getStateInferFromGrammarTable(pTokenHeadStack, pTokenInput);
				if (*(lstTokenInfer.end() - 1) == pTokenEpsilon) 
					lstTokenInfer.pop_back();
				
				lstTokenStack.pop_back();	// pop (X)
				reverse(lstTokenInfer.begin(), lstTokenInfer.end()); // dao nguoc state infer
				lstTokenStack.insert(lstTokenStack.end(), lstTokenInfer.begin(), lstTokenInfer.end());
			}
			else
			{
				bContinueParse = false;
				bRet = false;
			}
		}

	} while (bContinueParse);

_EXIT_FUNCTION:
	return bRet;
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

// Them token 
bool TokenManager::addToken(string sName)
{
	bool bCheck = false;
	if (isToken(sName) == false)
	{
		this->m_lstTokenName.push_back(sName);
		bCheck = true;
	}

	return bCheck;
}

// Them token 
bool TokenManager::addToken(vector<string> lstName) {
	bool bCheck = false;

	for (string sTokenName : lstName) {
		bool tmpCheck = this->addToken(sTokenName);
		bCheck = bCheck || tmpCheck;
	}
	return bCheck;
}

// Them luat sinh vao bang grammar 
bool TokenManager::addStateInferToGrammarTable(Tokener* pTokenNotTerminal, 
	Tokener* pTokenTerminal, int stateInferId) 
{
	bool bRet = true;
	unordered_map<Tokener*, int> stateInferNew;

	auto itCheckTokenNotTerminal = m_mapGrammarTable.find(pTokenNotTerminal);
	if (itCheckTokenNotTerminal != m_mapGrammarTable.end()) {
		unordered_map<Tokener*, int>& stateInferOld = m_mapGrammarTable[pTokenNotTerminal];
		auto itCheckTokenTerminal = stateInferOld.find(pTokenTerminal);
		if (itCheckTokenTerminal != stateInferOld.end()) {
			cout << "Warning: conflict in grammar table: row = " << pTokenNotTerminal->getName()
				<< "col = " << pTokenTerminal->getName() << endl;
			cout << "\t old state id = " << itCheckTokenTerminal->second
				<< "new state id = " << itCheckTokenTerminal->second << endl;
			bRet = false;
		}
	}
	m_mapGrammarTable[pTokenNotTerminal][pTokenTerminal] = stateInferId;
	/*else {
		stateInferNew[pTokenTerminal] = stateInferId;
		m_mapGrammarTable[pTokenNotTerminal] = stateInferNew;
	}*/
	return bRet;
}

// Kiem tra luat sinh tu bang grammar 
bool TokenManager::isContainInGrammarTable(Tokener* pTokenNotTerminal, Tokener* pTokenTerminal)
{
	bool bRet = false;
	
	auto itCheckRow = this->m_mapGrammarTable.find(pTokenNotTerminal);
	if (itCheckRow != m_mapGrammarTable.end()) {
		auto itCheckCol = itCheckRow->second.find(pTokenTerminal);
		if (itCheckCol != itCheckRow->second.end()) {
			bRet = true;
		}
	}
_EXIT_FUNCTION:
	return bRet;
}

// doc luat sinh tu bang grammar 
vector<Tokener*> TokenManager::getStateInferFromGrammarTable(Tokener* pTokenNotTerminal,Tokener* pTokenTerminal)
{
	vector<Tokener*> lstTokenInfer;
	if (isContainInGrammarTable(pTokenNotTerminal, pTokenTerminal) == false)
	{	
		// L?i 
		throw(new runtime_error("Err getStateInferFromGrammarTable(): table grammar not contain row = "
			+pTokenNotTerminal->getName() + " and col = " + pTokenTerminal->getName()));
	}
	else {
		auto itCheckRow = this->m_mapGrammarTable.find(pTokenNotTerminal);
		auto itCheckCol = itCheckRow->second.find(pTokenTerminal);
		int idStateInfer = itCheckCol->second;
		lstTokenInfer = pTokenNotTerminal->getListTokenInfer().at(idStateInfer);
	}
	return lstTokenInfer;
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

string TokenManager::generateNewName(string sName) {
	string sNewName = sName + "'";
	while (isToken(sNewName)) {
		sNewName += "'";
	}
	return sNewName;
}

 bool TokenManager::genListTokenInfer(vector<vector<string>> lstStateInferName, 
	 vector<vector<Tokener*>> & lstStateInfer)
{
	bool bRet = false;
		
	for (vector<string> lstToken : lstStateInferName)
	{
		vector<Tokener*> lstTokenPoint;
		for (string sToken : lstToken) {
			auto itToken = TokenManager::m_mapTokener.find(sToken);
			if (itToken != m_mapTokener.end()) {
				lstTokenPoint.push_back(itToken->second);
			}
			else {
				cout << "Err: Not found token object: " << sToken << endl;
				goto _EXIT_FUNCTION;
			}

		}
		lstStateInfer.push_back(lstTokenPoint);
	}
	bRet = true;
_EXIT_FUNCTION:
	return bRet;
}