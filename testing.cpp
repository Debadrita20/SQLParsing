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
    /*string ntsline="A B C RES DEL";
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
	cout<<"\n";*/
    string start_var="Q";
    queue<string> input, temp;
    string li,ans;
    input.push("$");
	stack<string> st;
	st.push("$");
	st.push(start_var);
    ans="";
    while(!st.empty() && !input.empty()) {
        if(input.front()=="") break;
		while(input.front() == st.top()) {
			st.pop();
            ans=ans+input.top();
			input.pop();
		}
		else {
			string stack_top = st.top();
			int row = distance(non_terms.begin(), non_terms.find(stack_top));
			int col = distance(terms.begin(), terms.find(input.front()));
			int prod_num = parse_table[row][col];

			if(prod_num == -2) {
                string expect="";
                auto it=terms.begin();
                for(int j=0;j<terms.size();j++)
                {
                    if(parse_table[row][j]>=0) 
                    { 
                        if(expect=="")
                        expect=expect+"\'"+(*it)+"\'";
                        else {
                        expect=expect+" or ";
                        expect=expect+"\'"+(*it)+"\'";
                        }
                    }
                    it++;
                }
                if(!rejectedex||input.front()!="$") {
                if(input.front()!="$")
				cout<<"Line "<<lineno.front()<<" Col. "<<colno.front()<<": Expected "<<expect<<" before "<<input.front()<<"\n";
                else cout<<"Line "<<prvl<<" Col. "<<prvc<<": Expected "<<expect<<"\n";
                }
                rejectedex=true;
				accepted = false;
                st.pop();
                continue;
			}
            else if(prod_num == -1) {
				cout<<"Line "<<lineno.front()<<" Col. "<<colno.front()<<" Unexpected token found: "<<input.front()<<"\n";
				accepted = false;
                input.pop();
                prvl=lineno.front();
                prvc=colno.front();
                lineno.pop();
                colno.pop();
                continue;
			}

			st.pop();
			string rhs = gram[prod_num].second;
            stringstream rhss(rhs);
            string w;
            vector<string> v;
            while(rhss>>w)
            {
                v.push_back(w);
            }


			if(v[0] == "epsilon") {
				continue;
			}
			for(auto ch = v.rbegin(); ch != v.rend(); ++ch) {
				st.push(*ch);
			}
		}
	}
    return 0;
}