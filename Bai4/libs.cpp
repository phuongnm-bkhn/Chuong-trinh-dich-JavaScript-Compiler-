
#include "libs.h"
#include <iostream>
#include <vector>
using namespace std;

vector<string> splitString(string sInput, string sDelimiter)
{

    vector<string> vtRet;
    int nPos = sInput.find(sDelimiter, 0);
    while (nPos != string::npos)
    {
        // ghi kq vao mang tra ve
        vtRet.push_back(sInput.substr(0, nPos));

        // Tang vi tri bat dau
        nPos = nPos + sDelimiter.size();
        if (nPos >= sInput.size())
            break;

        // Tiep tuc tim kiem vi tri tiep theo
        sInput = sInput.substr(nPos);
        nPos = sInput.find(sDelimiter, 0);
    }
    if (nPos == string::npos && sInput.size() > 0)
        vtRet.push_back(sInput);

    return vtRet;
}