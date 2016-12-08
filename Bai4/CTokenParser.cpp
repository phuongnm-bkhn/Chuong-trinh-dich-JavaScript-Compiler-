#include "CTokenParser.hpp"

TokenParser::TokenParser(string sToken){
    this->m_dataLine = sToken;
    this->m_bIsToken = false;
    this->m_bIsParsed = false;

    int nPos; 
    int nCountElement = 0;
    string sTmp = "";
    while(nCountElement < 2)
    {
        nPos = sToken.find("@");
        if(nPos != string::npos){
            sTmp = sToken.substr(0, nPos);
            if (nCountElement == 0){
                
                string parterm = string("")+ "%?[\\w:digit:]+";
                regex  parterm_check_block(parterm);    
                if(regex_match(sTmp, parterm_check_block) == false){
                    cout<<"Err Name: "<<sToken<<endl;
                    break;
                }

                this->m_sName = sTmp;
            }
            else if (nCountElement == 1){
                if(sTmp != "1" && sTmp != "0"){
                    cout<<"Err isToken: "<<sToken<<endl;
                    break;
                }
                this->m_bIsToken = ("1" == sTmp);
                sTmp = sToken.substr(nPos+1);
				this->m_sLexRegex = sTmp;
                this->m_bIsParsed = true;
                break;
            }
        }
        else 
            break;
        sToken = sToken.substr(nPos+1);
        nCountElement++;
    }
}

