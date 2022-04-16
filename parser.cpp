#include <bits/stdc++.h>

using namespace std;
/*vector<string> terminals;
class nonterminal
{
    string name;
    int isNullable;
    set<string> first;
    set<string> follow;
    vector<string> productions;
    public:
    nonterminal(string n)
    {
        name=n;
        isNullable=0;
    }
    void addproduction(string p)
    {
        productions.push_back(p);
        if(p=="epsilon") isNullable=1;
    }
};
vector<nonterminal> nonterminals;
unordered_map<string,unordered_map<string,int>> parsingtable;
string compute_first_set(string name)
{
    
    if(isTerminal(name)) first.insert(name);
    else
    {
        int i;
        for(i=0;i<nonterminals.size();i++)
        {
            if(nonterminals[i].getName()==name) break;
        }
        vector<string> prods=nonterminals[i].getProductions();
        for(int j=0;j<prods.size();j++)
        {
            string pr=prods[j];  
            stringstream ss(pr);
            string f;
            ss>>f;
            
        }
    }
}
void compute_follow_set()
{
    int i;
}
void parse()
{
    int i;
    //read nonterminals from file, store in fi_fo_set
    //read terminals from file
    for(i=0;i<;i++)
    { 
        compute_first_set(&nts[i]);
        compute_follow_set(&nts[i]);
    }
    generate_parsing_table(nts);
    string token;
    int line_no,col_no;
    //open tokens.txt
    while(!eof)
    {
        //read token,line_no,col_no
        //parse
        //error detection/recovery
    }
    //leftmost derivation display
    //parse tree drawing
}*/

vector< pair<string, string> > gram;
set<string> non_terms;
set<string> terms;
string start_var;
map< string, set<string> > firsts;
map< string, set<string> > follows;

void find_first(vector< pair<string, string> > gram, map< string, set<string> > &firsts, string non_term); 

void find_follow(vector< pair<string, string> > gram, map< string, set<string> > &follows, map< string, set<string> > firsts, 
	string non_term); 

int isTerminal(string s)
{
    return terms.count(s);
}

void parse()
{
	fstream grammar_file;
	grammar_file.open("testgrammar.txt", ios::in);
	if(grammar_file.fail()) {
		cout<<"The grammar file cannot be opened\n";
        return;
	}
    // Gather all non terminals
	string ntsline,tsline;
    getline(grammar_file,ntsline);
    stringstream ss(ntsline);
    string nt,t;
    while(ss>>nt)
    {
        non_terms.insert(nt);
    }
	cout<<"The non terminals are: ";
	for(auto i = non_terms.begin(); i != non_terms.end(); ++i) {
		cout<<*i<<" ";
	}
	cout<<"\n";
    // Gather all terminals
	getline(grammar_file,tsline);
    stringstream sst(tsline);
    while(sst>>t)
    {
        terms.insert(t);
    }
    terms.insert("$");
	cout<<"The terminals are: ";
	for(auto i = terms.begin(); i != terms.end(); ++i) {
		cout<<*i<<" ";
	}
	cout<<"\n";
	cout<<"Grammar parsed from grammar file: \n";
	
	int count = 0;
	while(!grammar_file.eof()) {
		string prod;
		getline(grammar_file,prod);
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
		pair <string, string> production (lhs, rhs);
		gram.push_back(production);
		cout<<count++<<".  "<<gram.back().first<<" -> "<<gram.back().second<<"\n";
	}
	cout<<"\n";

	for(auto non_term = non_terms.begin(); non_term != non_terms.end(); ++non_term) {
		if(firsts[*non_term].empty()){
			find_first(gram, firsts, *non_term);
		}
	}

	cout<<"First Sets for corresponding non-terminals: \n";
	for(auto it = firsts.begin(); it != firsts.end(); ++it) {
		cout<<it->first<<" : ";
		for(auto firsts_it = it->second.begin(); firsts_it != it->second.end(); ++firsts_it) {
			cout<<*firsts_it<<" ";
		}
		cout<<"\n";
	}
	cout<<"\n";

	// Follow of start variable first
	start_var = gram.begin()->first;
	follows[start_var].insert("$");
	find_follow(gram, follows, firsts, start_var);
	// Follows for rest of variables
	for(auto it = non_terms.begin(); it != non_terms.end(); ++it) {
		if(follows[*it].empty()) {
			find_follow(gram, follows, firsts, *it);
		}
	}

	cout<<"Follow Sets for the corresponding non-terminals: \n";
	for(auto it = follows.begin(); it != follows.end(); ++it) {
		cout<<it->first<<" : ";
		for(auto follows_it = it->second.begin(); follows_it != it->second.end(); ++follows_it) {
			cout<<*follows_it<<" ";
		}
		cout<<"\n";
	}
	cout<<"\n";

	int parse_table[non_terms.size()][terms.size()];
	fill(&parse_table[0][0], &parse_table[0][0] + sizeof(parse_table)/sizeof(parse_table[0][0]), -1);
	for(auto prod = gram.begin(); prod != gram.end(); ++prod) {
		string rhs = prod->second;
		set<string> next_list;
		bool ended = false;
        stringstream rhss(rhs);
        string s;
        while(rhss>>s)
        {
            if(isTerminal(s))
            {
                next_list.insert(s);
                ended=true;
                break;
            }
            if(s=="epsilon") continue;
            set<string> firsts_copy(firsts[s].begin(), firsts[s].end());
			if(firsts_copy.find("epsilon") == firsts_copy.end()) {
				next_list.insert(firsts_copy.begin(), firsts_copy.end());
				ended = true;
				break;
			}
			firsts_copy.erase("epsilon");
			next_list.insert(firsts_copy.begin(), firsts_copy.end());
        }
		// If the whole rhs can be skipped through epsilon or reaching the end
		// Add follow to next list
		if(!ended) {
			next_list.insert(follows[prod->first].begin(), follows[prod->first].end());
		}

		for(auto ch = next_list.begin(); ch != next_list.end(); ++ch) {
			int row = distance(non_terms.begin(), non_terms.find(prod->first));
			int col = distance(terms.begin(), terms.find(*ch));
			int prod_num = distance(gram.begin(), prod);
			if(parse_table[row][col] != -1) {
				cout<<"Collision at ["<<row<<"]["<<col<<"] for production "<<prod_num<<"\n";
				continue;
			}
			parse_table[row][col] = prod_num;
		}
	}
	// Print parsing table
	cout<<"LL(1) Parsing Table: \n";
	cout<<" \t";
	for(auto i = terms.begin(); i != terms.end(); ++i) {
		cout<<*i<<"\t";
	}
	cout<<"\n";
	for(auto row = non_terms.begin(); row != non_terms.end(); ++row) {
		cout<<*row<<"\t";
		for(int col = 0; col < terms.size(); ++col) {
			int row_num = distance(non_terms.begin(), row);
			if(parse_table[row_num][col] == -1) {
				cout<<"-\t";
				continue;
			}
			cout<<parse_table[row_num][col]<<"\t";
		}
		cout<<"\n";
	}
	cout<<"\n";

	queue<string> input,positions;
    string li;
    /*ifstream readFile("tokens.txt");
    while(getline(readFile,li))
    {
        stringstream str(li);
        string num,inp,pos,s;
        str>>num;
        str>>inp;
        input.push(inp);
        while(str>>s)
        {
            pos=pos+s;
            pos=pos+" ";
        }
        positions.push(pos);
    }*/
    input.push("id");
    input.push("*");
    input.push("(");
    input.push("id");
    input.push("+");
    input.push("id");
    input.push(")");
    input.push("$");
	stack<string> st;
	st.push("$");
	st.push(start_var);
   
	/*// Check if input string is valid
	for(auto ch = input.begin(); ch != input.end(); ++ch) {
		if(terms.find(*ch) == terms.end()) {
			cout<<"Input string is invalid\n";
			return;
		}
	}*/
	// cout<<"Processing input string\n";
	bool accepted = true;
	while(!st.empty() && !input.empty()) {
		if(input.front() == st.top()) {
			st.pop();
			input.pop();
		}
		else if(isTerminal(st.top())) {
			cout<<"Unmatched terminal found\n";
			accepted = false;
			break;
		}
		else {
			string stack_top = st.top();
			int row = distance(non_terms.begin(), non_terms.find(stack_top));
			int col = distance(terms.begin(), terms.find(input.front()));
			int prod_num = parse_table[row][col];

			if(prod_num == -1) {
				cout<<"No production found in parse table\n";
				accepted = false;
				break;
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

	if(accepted) {
		cout<<"No Syntax Errors Found...Input string is accepted\n";
	}
	else {
		cout<<"Input string is rejected\n";
	}

	return;
}

void find_first(vector< pair<string, string> > gram, map< string, set<string> > &firsts, string non_term) {

	for(auto it = gram.begin(); it != gram.end(); ++it) {
		// Find productions of the non terminal
		if(it->first != non_term) {
			continue;
		}

		string rhs = it->second;
        stringstream rhss(rhs);
        string w;
        vector<string> v;
        while(rhss>>w)
        {
            v.push_back(w);
        }
		// Loop till a non terminal or no epsilon variable found
		for(auto ch = v.begin(); ch != v.end(); ++ch) {
			if(isTerminal(*ch)||(*ch)=="epsilon") {
				firsts[non_term].insert(*ch);
				break;
			}
			else {
				// If char in prod is non terminal and whose firsts has no yet been found out
				// Find first for that non terminal
				if(firsts[*ch].empty()) {
					find_first(gram, firsts, *ch);
				}
				// If variable doesn't have epsilon, stop loop
				if(firsts[*ch].find("epsilon") == firsts[*ch].end()) {
					firsts[non_term].insert(firsts[*ch].begin(), firsts[*ch].end());
					break;
				}

				set<string> firsts_copy(firsts[*ch].begin(), firsts[*ch].end());

				// Remove epsilon from firsts if not the last variable
				if(ch + 1 != v.end()) {
					firsts_copy.erase("epsilon");
				}

				// Append firsts of that variable
				firsts[non_term].insert(firsts_copy.begin(), firsts_copy.end());
			}
		}
		
	}
}

void find_follow(vector< pair<string, string> > gram, map< string, set<string> > &follows, map< string,set<string> > firsts, 
	string non_term) {

	for(auto it = gram.begin(); it != gram.end(); ++it) {

		// finished is true when finding follow from this production is complete
		bool finished = true;
		string rhs = it->second;
        stringstream rhss(rhs);
        string w;
        vector<string> v;
        while(rhss>>w)
        {
            v.push_back(w);
        }
		// Skip variables till reqd non terminal
        auto ch=v.begin();
		for(;ch != v.end() ; ++ch) {
			if(*ch == non_term) {
				finished = false;
				break;
			}
		}
		++ch;

		for(;ch != v.end() && !finished; ++ch) {
			if(isTerminal(*ch)||(*ch)=="epsilon") {
				follows[non_term].insert(*ch);
				finished = true;
				break;
			}

			set<string> firsts_copy(firsts[*ch]);
			// If char's firsts doesnt have epsilon follow search is over 
			if(firsts_copy.find("epsilon") == firsts_copy.end()) {
				follows[non_term].insert(firsts_copy.begin(), firsts_copy.end());
				finished = true;
				break;
			}
			// Else next char has to be checked after appending firsts to follow
			firsts_copy.erase("epsilon");
			follows[non_term].insert(firsts_copy.begin(), firsts_copy.end());

		}


		// If end of production, follow same as follow of variable
		if(ch == v.end() && !finished) {
			// Find follow if it doesn't have
			if(follows[it->first].empty()) {
				find_follow(gram, follows, firsts, it->first);
			}
			follows[non_term].insert(follows[it->first].begin(), follows[it->first].end());
		}

	}

}
int main()
{
    cout<<"unit testing\n";
    parse();
    return 0;
}