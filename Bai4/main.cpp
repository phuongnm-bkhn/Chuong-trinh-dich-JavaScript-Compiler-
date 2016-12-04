#include <iostream>
#include <map>
#include <stdio.h>
#include <vector>
using namespace std;

#include "CLexer.h"
#include "CTokener.hpp"
#include "CTokenManager.hpp"
#include "libs.h"

int main(int argc, char **argv)
{
    vector<TOKEN_ID> lstToken = getListToken("data/code.js");
    TokenManager* tokenManager = new TokenManager("data/grammar-def");

    for (int i = 0; i < lstToken.size(); i++)
    {
        TOKEN_ID id = lstToken[i];
        printf("\ntoken => %s", g_mapTokenName[id].c_str());
    }
    getchar();
}
