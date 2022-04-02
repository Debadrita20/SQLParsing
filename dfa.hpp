
#include <bits/stdc++.h>

using namespace std;

class state
{
    private: 
    int isFinal;
    int stateNo;
    string tokenclass;
    vector<pair<int,state>> transitions;
    public:
    state(int sn)
    {
        isFinal=false;
        stateNo=sn;
        tokenclass="";
    }
    void setFinal()
    {
        isFinal=true;
    }
    void setTokenClass(string s)
    {
        tokenclass=s;
    }
    string getTokenClass()
    {
        return tokenclass;
    }
    void addTransition(int i,state ns)
    {
        pair<int,state> p={i,ns};
        transitions.push_back(p);
    }
    int getNo()
    {
        return stateNo;
    }

    vector<pair<int,state>> getTransitions(){
        return transitions;
    }

    state next(int i)
    {
        int j;
        for(j=0;j<transitions.size();j++)
        {
            if(transitions[j].first==i) return transitions[j].second;
        }
        return state(-1);
    }
};
class dfa
{
    private:
    int startno;
    vector<state> statelist;
    public:
    dfa()
    {
        startno=0;
    }
    void setStart(int st)
    {
        startno=st;
    }
    int addState(int isFin,string s)
    {
        state *news=new state(statelist.size());
        if(isFin) news->setFinal();
        news->setTokenClass(s);
        statelist.push_back(*news);
        return news->getNo();
    }
    void addTransition(int fromstate,int newstate,int sym)
    {
        statelist[fromstate].addTransition(sym,statelist[newstate]);
    }
    int getStart()
    {
        return startno;
    }
    state * getState(int n)
    {
        return &statelist[n];
    }

    int canExtend(int csn,int i)
    {
        int flag=0;
        state curState = statelist[csn];
        for (int j = 0; j < curState.getTransitions().size(); j++){
            if (curState.getTransitions()[j].first == i){
                flag = 1;
                break;
            }
        }
        return flag;
    }
};
