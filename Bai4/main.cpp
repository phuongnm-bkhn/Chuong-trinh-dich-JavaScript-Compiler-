#include <iostream>
#include <map>
#include <stdio.h>
#include <vector>
#include <set>
using namespace std;

#include "CLexer.h"
#include "CTokener.hpp"
#include "CTokenManager.hpp"
#include "libs.h"

int main(int argc, char **argv)
{
	// codejs la file chua ma nguon 
    vector<string> lstToken = getListToken("data/code.js");
    TokenManager* tokenManager = new TokenManager("data/grammar-def");
	
  //  for (int i = 0; i < lstToken.size(); i++)
  //  {
  //      string sTokenName = lstToken[i];
  //      //printf("\ntoken => %s", g_mapTokenName[id].c_str());
		//printf("\ntoken => %s", sTokenName);
  //  }
	bool bRet = tokenManager->tryParse(lstToken);
	cout << endl << endl;
	if(bRet)
		cout << "True: Van ban input phu hop luat sinh !!" << endl;
	else
		cout << "False: Van ban input khong phu hop luat sinh !!" << endl;

    getchar();
	delete(tokenManager);
}
