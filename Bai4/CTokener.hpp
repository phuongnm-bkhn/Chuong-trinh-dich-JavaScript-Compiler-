#ifndef C_TOKENER_H
#define C_TOKENER_H

#include <fstream>
#include <iostream>
#include <map>
#include <streambuf>
#include <vector>
using namespace std;
#include "libs.h"

extern vector<string> splitString(string sInput, string sDelimiter);

class Tokener
{
  public:
    Tokener(string sTokenDefine);
  private:
    string m_sTokenDefine;
};

#endif