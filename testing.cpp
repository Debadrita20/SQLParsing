#include <bits/stdc++.h>

using namespace std;

int main()
{
    /*string prod="RESS -> id B C";
    stringstream ss(prod);
    string lhs;
    ss>>lhs;
    string arrow;
    ss>>arrow;
    string x,rhs;
    while(ss>>x)
    {
        rhs=rhs+x;
        rhs=rhs+" ";
    }
    cout<<lhs<<" "<<" "<<rhs<<endl;*/
    string ntsline="A B C RES DEL";
    stringstream ss(ntsline);
    string nt;
    set<string> non_terms;
    while(ss>>nt)
    {
        non_terms.insert(nt);
    }
	cout<<"The non terminals in the grammar are: ";
	for(auto i = non_terms.begin(); i != non_terms.end(); ++i) {
		cout<<*i<<" ";
	}
	cout<<"\n";
    return 0;
}