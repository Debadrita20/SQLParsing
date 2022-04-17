#include <bits/stdc++.h>
//#include "parser_initialize.hpp"
using namespace std;
class parser
{
public:
vector< pair<string, string> > gram;
set<string> non_terms;
set<string> terms;
string start_var;
map< string, set<string> > firsts;
map< string, set<string> > follows;
int **parse_table;
public:
//void find_first(vector< pair<string, string> > gram, map< string, set<string> > &firsts, string non_term); 

//void find_follow(vector< pair<string, string> > gram, map< string, set<string> > &follows, map< string, set<string> > firsts, 
//	string non_term); 
parser()
{
    start_var="";
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
int isTerminal(string s)
{
    return terms.count(s);
}
void initialize()
{
    fstream grammar_file;
	grammar_file.open("grammar.txt", ios::in);
	if(grammar_file.fail()) {
		cout<<"The grammar file cannot be opened\n";
        return;
	}
    fstream parsing_req;
    parsing_req.open("parsing_req.txt",ios::out);
    // Gather all non terminals
	string ntsline,tsline;
    getline(grammar_file,ntsline);
    stringstream ss(ntsline);
    string nt,t;
    while(ss>>nt)
    {
        non_terms.insert(nt);
    }
	parsing_req<<"The non terminals are: ";
	for(auto i = non_terms.begin(); i != non_terms.end(); ++i) {
		parsing_req<<*i<<" ";
	}
	parsing_req<<"\n";
    // Gather all terminals
	getline(grammar_file,tsline);
    stringstream sst(tsline);
    while(sst>>t)
    {
        terms.insert(t);
    }
    terms.insert("$");
	parsing_req<<"The terminals are: ";
	for(auto i = terms.begin(); i != terms.end(); ++i) {
		parsing_req<<*i<<" ";
	}
	parsing_req<<"\n";
	parsing_req<<"Grammar parsed from grammar file: \n";
	
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
		parsing_req<<count++<<".  "<<gram.back().first<<" -> "<<gram.back().second<<"\n";
	}
	parsing_req<<"\n";

	for(auto non_term = non_terms.begin(); non_term != non_terms.end(); ++non_term) {
		if(firsts[*non_term].empty()){
			find_first(gram, firsts, *non_term);
		}
	}

	parsing_req<<"First Sets for corresponding non-terminals: \n";
	for(auto it = firsts.begin(); it != firsts.end(); ++it) {
		parsing_req<<it->first<<" : ";
		for(auto firsts_it = it->second.begin(); firsts_it != it->second.end(); ++firsts_it) {
			parsing_req<<*firsts_it<<" ";
		}
		parsing_req<<"\n";
	}
	parsing_req<<"\n";

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

	parsing_req<<"Follow Sets for the corresponding non-terminals: \n";
	for(auto it = follows.begin(); it != follows.end(); ++it) {
		parsing_req<<it->first<<" : ";
		for(auto follows_it = it->second.begin(); follows_it != it->second.end(); ++follows_it) {
			parsing_req<<*follows_it<<" ";
		}
		parsing_req<<"\n";
	}
	parsing_req<<"\n";

	parse_table=(int **)malloc(non_terms.size()*sizeof(int *));
    for(int kk=0;kk<non_terms.size();kk++)
    {
        parse_table[kk]=(int *)malloc(terms.size()*sizeof(int));
    }
    for(int kk=0;kk<non_terms.size();kk++)
    {
        for(int kkk=0;kkk<terms.size();kkk++)
        {
            parse_table[kk][kkk]=-1;
        }
    }
	//fill(&parse_table[0][0], &parse_table[0][0] + sizeof(parse_table)/sizeof(parse_table[0][0]), -1);
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
    //for error recovery
    for(auto row = non_terms.begin(); row != non_terms.end(); ++row) {
        auto i=terms.begin();		
		for(int col = 0; col < terms.size(); ++col) {
			int row_num = distance(non_terms.begin(), row);
			if(parse_table[row_num][col] == -1) {
				if(*(i)=="$"||follows[(*row)].count(*(i)))
                parse_table[row_num][col]=-2;    //-2 for pop, -1 for scan
                i++;
				continue;
			}
            i++;			
		}		
	}
	// Print parsing table
	parsing_req<<"LL(1) Parsing Table: \n";
	parsing_req<<" \t";
	for(auto i = terms.begin(); i != terms.end(); ++i) {
		parsing_req<<*i<<"\t";
	}
	parsing_req<<"\n";
	for(auto row = non_terms.begin(); row != non_terms.end(); ++row) {
		parsing_req<<*row<<"\t";
		for(int col = 0; col < terms.size(); ++col) {
			int row_num = distance(non_terms.begin(), row);
			if(parse_table[row_num][col] == -1) parsing_req<<"scan\t";
            else if(parse_table[row_num][col] == -2) parsing_req<<"pop\t";
			else parsing_req<<parse_table[row_num][col]<<"\t";
		}
		parsing_req<<"\n";
	}
	parsing_req<<"\n";
    grammar_file.close();
    parsing_req.close();
}
void parse(vector<pair<vector<string>,vector<pair<int,int>>>> toks,vector<int> hasLexErr)
{
    for(int i=0;i<toks.size();i++) {
        //cout<<"Query "<<(i+1)<<endl;
    if(hasLexErr[i]) {
        cout<<"SQL Query "<<(i+1)<<" Rejected"<<endl;
        continue;
    }
    queue<string> input, temp;
    queue<int> lineno,colno;
    string li;
    /*ifstream readFile("tokens.txt");
    while(getline(readFile,li))
    {
        stringstream str(li);
        string num,inp,pos,s;
        str>>num;
        str>>inp;
        input.push(inp);
        str>>s;
        pos=pos+s;
        pos=pos+" Col. ";
        str>>s;
        pos=pos+s;
        positions.push(pos);
    }*/
    vector<string> in=toks[i].first;
    for(int j=0;j<in.size();j++)
    input.push(in[j]);
    vector<pair<int,int>> po=toks[i].second;
    for(int j=0;j<po.size();j++)
    {
        lineno.push(po[j].first);
        colno.push(po[j].second);
    }
    input.push("$");
	stack<string> st;
	st.push("$");
	st.push(start_var);

	temp = input;
    int prvl=0,prvc=0;
	bool accepted = true,rejectedex=false;
	while(!st.empty() && !input.empty()) {
        if(input.front()=="") break;
		if(input.front() == st.top()) {
			st.pop();
			input.pop();
            prvl=lineno.front();
            prvc=colno.front();
            lineno.pop();
            colno.pop();
		}
		else if(isTerminal(st.top())) {
			if(st.top()!="$") cout<<"Unmatched terminal: "<<st.top()<<"\n";
            else cout<<"Encountered unmatched tokens in input"<<"\n";
			accepted = false;
			st.pop();
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

	input = temp;
    if(accepted) {
		cout<<"SQL Query "<<(i+1)<<" Accepted...no errors found\n";
	}
	else {
		cout<<"Errors Found in SQL Query "<<(i+1)<<"...Rejected\n";
	}

    // LMDT
	if (accepted){

		int row = distance(non_terms.begin(), non_terms.find(start_var));
		int col = distance(terms.begin(), terms.find(input.front()));
		int prod_num = parse_table[row][col];
        string output = "";
		output = gram[prod_num].second;
        cout<<"Leftmost Derivation:~\n";
        cout<<start_var<<"\n";
		cout << "=> " << output << " " << endl << "=>";

        while (true){

			stringstream ss (output);
			string sym;

			int nonTerminalPresent = 0;
			int f = 0;
			string newoutput = "";
			
			while (ss >> sym){
				// cout << "Symbol : " << ++count << endl;
				if (isTerminal(sym)){
					cout << " " << sym;
					newoutput += " ";
					newoutput += sym;
					
					continue;
				}

				if (f == 1){   // ckeck for first non terminal in the rule
				    
					// -------------------------------------->

					if (sym != "epsilon"){
						cout << " " << sym; 
					
						newoutput += " ";
						newoutput += sym;
					}
				
					
					// cout << "no " << newoutput << " ";
					continue;
				}

				// if sym is not terminal
				row = distance(non_terms.begin(), non_terms.find(sym));
				col = distance(terms.begin(), terms.find(input.front()));
				prod_num = parse_table[row][col];

				string children = gram[prod_num].second;

				stringstream schild(children);
				string child;

				while(schild >> child){
					if (isTerminal(child)){
						input.pop();
					}
					
					
					else {
						break;
					}
				}


				nonTerminalPresent = 1;

				stringstream sschild(children);

				// print chidren
				while(sschild >> child){
					
	//  ------------------------------------------->
					if (child != "epsilon"){
						cout << " " << child;
					    newoutput += " ";
					    newoutput += child;
					
					}
				}
				
				f = 1;      // Encountered first non terminal
			}

			output = newoutput;

			cout << endl;
			
            
			stringstream scheck(newoutput);
			string check;
			int c = 0;
			while (scheck >> check){
				if (!isTerminal(check)){
					c = 1;
					break;
				}
			}

			if (c == 0){
				break;
			}

			cout << "=>";

			if (nonTerminalPresent == 0){
				break;
			}

		}
	}

    //readFile.close();
    }
    //cout<<"tata"<<endl;
    return;
}
};
