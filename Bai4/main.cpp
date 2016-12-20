#include <iostream>
#include <map>
#include <set>
#include <stdio.h>
#include <vector>
using namespace std;

#include "CLexer.h"
#include "CTokenManager.hpp"
#include "CTokener.hpp"
#include "libs.h"

int main(int argc, char **argv)
{
    // codejs la file chua ma nguon
    vector<string> lstToken = getListToken("data/code.js");
    TokenManager *tokenManager = new TokenManager("data/grammar-def");

    bool bRet = tokenManager->tryParse(lstToken);
    cout << endl
         << endl;

    if (bRet)
        cout << "True: Van ban input phu hop luat sinh !!" << endl;
    else
        cout << "False: Van ban input khong phu hop luat sinh !!" << endl;

    getchar();
    delete (tokenManager);
}
