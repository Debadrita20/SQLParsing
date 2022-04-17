
#include <bits/stdc++.h>
#include "keyword_fn_list_initialise.hpp"

using namespace std;
vector<string> keywords;
vector<string> fnames;
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
pair<vector<string>,vector<pair<int,int>>> generateTokens(dfa *myDFA,string query, unordered_map<char,queue<pair<int,int>>> pos, unordered_map<string,stack<pair<string,pair<int,int>>>> &symbol_table)
{
    string curLexeme="";
    int curState=myDFA->getStart();
    char lookaheadChar;
    int inputSymbol;
    vector<string> tokens;
    initialise_k(&keywords);
    initialise_fn(&fnames);
    pair<vector<string>,vector<pair<int,int>>> pp;
    vector<string> tt;
    vector<pair<int,int>> posi;
    //code to open file for storing the tokens and token numbers
    ofstream fp("tokens.txt");
    //cout<<query<<endl;
    query=query+" ";
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
            if(tokenclass=="error")
            {
                if(curState==4)
                cout<<"Line "<<pos[curLexeme[curLexeme.size()-1]].front().first<<" Col. "<<pos[curLexeme[curLexeme.size()-1]].front().second<<": Expected digit after ."<<endl;
                else if(curState==6||curState==7)
                cout<<"Line "<<pos[curLexeme[curLexeme.size()-1]].front().first<<" Col. "<<pos[curLexeme[curLexeme.size()-1]].front().second<<": Expected closing ' after "<<curLexeme[curLexeme.size()-1]<<""<<endl;
                curLexeme="";
                for(int j=0;j<curLexeme.size();j++)
                pos[curLexeme[j]].pop();
                i--;  //for checking lookahead char again
                curState=myDFA->getStart();
                continue;
            }
            int isKeyword=0,isFn_name=0;
            if(tokenclass.compare("id")==0)
            {
                isKeyword=checkKeywordList(toUpper(curLexeme));
                if(!isKeyword)
                isFn_name=checkFnNameList(toUpper(curLexeme));
            }
            if(isKeyword) tokenclass=toUpper(curLexeme)+"_KEYWORD";
            else if(isFn_name) tokenclass="fn_name";
            else if(toUpper(curLexeme)=="NOT") tokenclass="not_operator";
            else if(toUpper(curLexeme)=="AND"||toUpper(curLexeme)=="OR") tokenclass="binary_logical_operator";
            tokens.push_back(tokenclass);  
            fp<<tokens.size()<<" "<<tokenclass<<" "<<pos[curLexeme[0]].front().first<<" "<<pos[curLexeme[0]].front().second<<"\n"; 
            tt.push_back(tokenclass);
            /*string po=""+pos[curLexeme[0]].front().first;
            po=po+" ";
            po=po+""+pos[curLexeme[0]].front().second;
            posi.push_back(po);*/
            //put lexeme in symbol table with the tokenclass and token/line number
            //code 
           // cout<<"\n\n\n\n";

           /* for(auto k=pos.begin();k!=pos.end();k++){
                cout<<k->first<<"   test  "<< k->second.size()<<"   test  ";
                while(!k->second.empty()){
                    cout<<k->second.front().first<<" "<<k->second.front().second<<"     ";
                    k->second.pop();
                }
                cout<<endl;
            }*/
            symbol_table[curLexeme].push({tokens[tokens.size()-1],{pos[curLexeme[0]].front().first,pos[curLexeme[0]].front().second}});
            
            //cout<< curLexeme<<" "<<tokens[tokens.size()-1]<<" "<<pos[curLexeme[0]].front().first<<" "<<pos[curLexeme[0]].front().second<<"\n\n\n";
            for(int j=0;j<curLexeme.size();j++)
                pos[curLexeme[j]].pop();
            curLexeme="";
            i--;  //for checking lookahead char again
            curState=myDFA->getStart();
        }
        else if(lookaheadChar==' ')
        continue;
        else   
        {
            //lexical error recovery code
            cout<<"Line "<<pos[lookaheadChar].front().first<<" Col. "<<pos[lookaheadChar].front().second<<" Unidentified character in SQL query: "<<lookaheadChar<<""<<endl;
            pos[lookaheadChar].pop();
            curState=myDFA->getStart();
        }
    }
    //unordered_map<string,stack<pair<string,pair<int,int>>>> symbol_table;
    /*for(int i=0;i<tokens.size();i++)
    {
        cout<<tokens[i]<<endl;
    }*/
    fp.close();
    pp=make_pair(tt,posi);
    return pp;
}