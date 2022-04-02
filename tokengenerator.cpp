
#include <bits/stdc++.h>

using namespace std;
void initialise_k(vector<string> kk)
{
    kk.push_back("SELECT");
    kk.push_back("FROM");
    kk.push_back("WHERE");
    kk.push_back("GROUP");
    kk.push_back("BY");
    kk.push_back("HAVING");
    kk.push_back("ORDER");
    kk.push_back("ASC");
    kk.push_back("DESC");
    kk.push_back("AS");
    kk.push_back("INSERT");
    kk.push_back("INTO");
    kk.push_back("VALUES");
    kk.push_back("DELETE");
}
void initialise_fn(vector<string> ff)
{
    ff.push_back("SUM");
    ff.push_back("COUNT");
    ff.push_back("MAX");
    ff.push_back("MIN");
    ff.push_back("AVG");
    ff.push_back("STDDEV");
    ff.push_back("VARIANCE");
    //ff.push_back("FIRST");
    //ff.push_back("LAST");
}
void generateTokens(dfa *myDFA,string query)
{
    string curLexeme="";
    int curState=myDFA->getStart();
    char lookaheadChar;
    int inputSymbol;
    vector<string> keywords;
    vector<string> fnames;
    vector<string> tokens;
    initialise_k(keywords);
    initialise_fn(fnames);
    //code to open file for storing the tokens and token numbers
    for(int i=0;i<query.size();i++)
    {
        lookaheadChar=query[i];
        if((lookaheadChar>='a'&&lookaheadChar<='z') || (lookaheadChar>='A'&&lookaheadChar<='Z'))
        inputSymbol=1;
        else if(lookaheadChar>='0'&&lookaheadChar<='9')
        inputSymbol=2;
        else
        inputSymbol=(int)lookaheadChar;
        if(myDFA->canExtend(curState,inputSymbol))
        {
            curState=myDFA->getState(curState)->next(inputSymbol).getNo();
            curLexeme=curLexeme+lookaheadChar;
        }
        else if(curLexeme.size()>0)
        {
            tokens.push_back(myDFA->getState(curState)->getTokenClass());
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