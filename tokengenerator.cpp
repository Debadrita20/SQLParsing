
#include <bits/stdc++.h>

using namespace std;
void generateTokens(dfa *myDFA,string query)
{
    string curLexeme="";
    int curState=myDFA->getStart();
    char lookaheadChar;
    int inputSymbol;
    vector<string> tokens;
    //code to open file for storing the tokens and token numbers
    for(int i=0;i<query.size();i++)
    {
        lookaheadChar=query[i];
        if(lookaheadChar>='A'&&lookaheadChar<='Z')
        inputSymbol=1;
        else if(lookaheadChar>='0'&&lookaheadChar<='9')
        inputSymbol=2;
        else
        inputSymbol=(int)lookaheadChar;
        if(myDFA->canExtend(curState,inputSymbol))
        {
            curState=myDFA->getState(curState).next(inputSymbol).getNo();
            curLexeme=curLexeme+lookaheadChar;
        }
        else if(curLexeme.size()>0)
        {
            tokens.push_back(curLexeme);
            curLexeme="";
            //for checking lookahead char again
            i--;
            curState=myDFA->getStart();
        }
        else if(lookaheadChar==' ')
        continue;
        else   
        {
            //lexical error recovery code
            cout<<"Unidentified character in SQL query: "<<lookaheadChar<<endl;
        }
    }
}