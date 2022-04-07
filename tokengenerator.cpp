
#include <bits/stdc++.h>

using namespace std;
vector<string> keywords;
vector<string> fnames;
void initialise_k()
{
    keywords.push_back("SELECT");
    keywords.push_back("FROM");
    keywords.push_back("WHERE");
    keywords.push_back("GROUP");
    keywords.push_back("BY");
    keywords.push_back("HAVING");
    keywords.push_back("ORDER");
    keywords.push_back("ASC");
    keywords.push_back("DESC");
    keywords.push_back("AS");
    keywords.push_back("INSERT");
    keywords.push_back("INTO");
    keywords.push_back("VALUES");
    keywords.push_back("DELETE");
}
void initialise_fn()
{
    fnames.push_back("SUM");
    fnames.push_back("COUNT");
    fnames.push_back("MAX");
    fnames.push_back("MIN");
    fnames.push_back("AVG");
    fnames.push_back("STDDEV");
    fnames.push_back("VARIANCE");
    //fnames.push_back("FIRST");
    //fnames.push_back("LAST");
}
int checkFnNameList(string s)
{
    for(int i=0;i<fnames.size();i++)
    {
        if(s.compare(fnames[i])==0) return 1;
    }
    return 0;
}
int checkKeywordList(string s)
{
    for(int i=0;i<keywords.size();i++)
    {
        if(s.compare(keywords[i])==0) return 1;
    }
    return 0;
}
string toUpper(string s)
{
    string result;
    for(int i=0;i<s.size();i++)
    {
        if(s[i]>='a'&&s[i]<='z')
        result=result+(char)(s[i]-32);
        else result=result+s[i];
    }
    return result;
}
void generateTokens(dfa *myDFA,string query)
{
    string curLexeme="";
    int curState=myDFA->getStart();
    char lookaheadChar;
    int inputSymbol;
    vector<string> tokens;
    initialise_k();
    initialise_fn();
    /*for(int i=0;i<keywords.size();i++)
    {
        cout<<keywords[i]<<endl;
    }
    for(int i=0;i<fnames.size();i++)
    {
        cout<<fnames[i]<<endl;
    }*/

    //code to open file for storing the tokens and token numbers

    //cout<<query<<endl;
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
            string tokenclass=myDFA->getState(curState)->getTokenClass();
            int isKeyword=0,isFn_name=0;
            if(tokenclass.compare("id")==0)
            {
                isKeyword=checkKeywordList(toUpper(curLexeme));
                if(!isKeyword)
                isFn_name=checkFnNameList(toUpper(curLexeme));
            }
            if(isKeyword) tokenclass=toUpper(curLexeme)+"_KEYWORD";
            else if(isFn_name) tokenclass="fn_name";
            tokens.push_back(tokenclass);  
            //put lexeme in symbol table with the tokenclass and token/line number
            //code          
            curLexeme="";
            i--;  //for checking lookahead char again
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
    //following code is for the last lexeme at the end of the query which is in current lexeme when the loop ends
    if(curLexeme.size()>0)
    {
        string tokenclass=myDFA->getState(curState)->getTokenClass();
        int isKeyword=0,isFn_name=0;
        if(tokenclass.compare("id")==0)
        {
            isKeyword=checkKeywordList(toUpper(curLexeme));
            if(!isKeyword)
            isFn_name=checkFnNameList(toUpper(curLexeme));
        }
        if(isKeyword) tokenclass=toUpper(curLexeme)+"_KEYWORD";
        else if(isFn_name) tokenclass="fn_name";
        tokens.push_back(tokenclass);  
        //put lexeme in symbol table with the tokenclass and token/line number
        //code          
    }
    else   
    {
        //lexical error recovery code
        cout<<"Unidentified character in SQL query: "<<lookaheadChar<<endl;
    }
    for(int i=0;i<tokens.size();i++)
    {
        cout<<tokens[i]<<endl;
    }
}