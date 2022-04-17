
#include <bits/stdc++.h>
#include "dfa.hpp"
#include "tokengenerator.cpp"
#include "parser.cpp"
#include "dfa_initialize.hpp"
//#include "parser_initialize.hpp"

using namespace std;
vector<unordered_map<char,queue<pair<int,int>>>> pos;
unordered_map<string,stack<pair<string,pair<int,int>>>> symbol_table;
int firstsemioccur(string input){
    for(int i=0;i<input.size();i++){
        if(input[i]==';')
            return i;
    }
    return -1;
}
string removeextraspace(string str){
    string nstr;
    for(int i=0; i<str.length();  ){
        
       
        if(str[i] == ' '){
            /* 
              *do not include the white space, if-
              *it is at the trailing or leading position
            */
            if(i==0 || i==str.length()-1){
                i++;
                continue;
            }
            
            /*
              *if space is inbetween then skip it-
              *except the last occurrence
            */
            while(str[i+1] == ' ')
                i++;
        }
        
        //concatenate the character to the new string
        nstr += str[i++];
    }
    return nstr;
}
string erasesubstr(string s,int k,int j){
    string res;
    for(int i=0;i<k;i++)
        res+=s[i];
    int i=0,x=j;
    for( i=0;i<(s.size()-x-1);i++){
        res+=s[j+1];
        j++;
    }
    return res;
}
void remove_multiline_comment_from_middle(vector<string>& query){
    for(int i=0;i<query.size();i++){
        int k=0;
      int p=0;
        for(int j=0;j<query[i].size();){
            if((int)query[i][j]==47 && (int)query[i][j+1]==42){
               
                k=j;
                
                while(!((int)query[i][j]==42 && (int)query[i][j+1]==47)){
                    if(j>=query[i].size()-2)
                        break;
                    j++;
                    
                }
                //j=4
                query[i]=erasesubstr(query[i],k, j+1);
                j=k+1;
            }
            else{
                j++;
            }
        }
    }
}
int main(int argc, char** argv)
{
    string ans;
    vector<string> query;
    dfa *myDFA=new dfa();
    parser *myParser=new parser();
    //initialise myDFA
    initialiseDFA(myDFA);
    //initialise myParser
    myParser->initialize();
    unordered_map<char,queue<pair<int,int>>> pos0;
    if(argc==1){
        
        
            //cout <<"hii";

            string input,s;
            char c,prev;
            int i=0,row=0,open=0;
            while (1)
            {
                getline(cin,input);
                row++;
                
                //cout<< input<<endl;
                
                int found = input.find("--");
                if (found != string::npos){
                    input.erase(input.begin() + found, input.end());
                }
                if(input.size()==0) continue;
                int j1;
                for(j1=0;j1<input.size()-1;  ){
                    if(input[j1]=='/' && input[j1+1]=='*'){
                        open=1;
                    }
                    if(open==0 && input[j1]!=' '){
                        pos0[input[j1]].push({row,j1+1});
                    }
                    if(input[j1]=='*' && input[j1+1]=='/' && open==1){
                        open=2;
                    }
                    if(input[j1]=='/' && open==2){
                        open=0;
                    }
                    j1++;
                }
                if(open==0 && input.back()!=' '){
                    pos0[input.back()].push({row,input.size()});
                }
                if(input[input.size()-1]=='/' && open==2){
                        open=0;
                }
                if(i>0)
                    s += (" "+input);
                else
                    s+=input;
                i++;
                int x=firstsemioccur(input);
                if(x!=-1){
                   // input=input.substr(0,x-1);
                    break;
                }
            }
         //   cout<<"\n\n\n";
         //   cout<< pos.size()<<"    xyz";
          /*    for(auto f=pos0.begin();f!=pos0.end();f++){
                    cout<<f->first<<"    ";
                    while(!f->second.empty()){
                        cout<<f->second.front().first<<" "<<f->second.front().second<<"    ";
                        f->second.pop();
                    }
                    cout<<endl;
                }*/
            pos.push_back(pos0);
            s=removeextraspace(s);
           // cout<< s<<endl;

            ans+=s;
           
        //cout << ans;
    }
    else if(argc==2){
        // Creation of ifstream class object to read the file
        ifstream input_file(argv[1]);
        string line;
        // by default open mode = ios::in mode
       // fin.open(argv[1]);
       if (!input_file.is_open()) {
        cerr << "Could not open the file - '"
             << argv[1] << "'" << endl;
        return EXIT_FAILURE;
         }

        unordered_map<char,queue<pair<int,int>>> pos0;
        int i=0;
        string s;
        // Execute a loop until EOF (End of File)
        int cx=0,open=0;
        while (getline(input_file, line)) {
            cx++;
            // Read a Line from File
          //  getline(fin, line);
           // cout<<line<<endl;
            int found = line.find("--");
            if (found != string::npos){
                line.erase(line.begin() + found, line.end());
            }
            int j1; 
            if(line.size()==0){
                continue;
            }
            for(j1=0;j1<line.size()-1;  ){
                if(line[j1]=='/' && line[j1+1]=='*'){
                    open=1;
                }
                if(open==0 && line[j1]!=' '){
                    pos0[line[j1]].push({cx,j1+1});
                }
                if(line[j1]=='*' && line[j1+1]=='/' && open==1){
                    open=2;
                }
                if(line[j1]=='/' && open==2){
                    open=0;
                }
                if(line[j1]==';' && open==0){
                  
                  
                    pos.push_back(pos0);
                    pos0.clear();
                }
                j1++;
            }
            if(open==0 && line.back()!=' '){
                pos0[line.back()].push({cx,line.size()});
                if(line.back()==';'){
                    pos.push_back(pos0);
                  /*  for(auto f=pos0.begin();f!=pos0.end();f++){
                        cout<<f->first<<"    ";
                        while(!f->second.empty()){
                            cout<<f->second.front().first<<" "<<f->second.front().second<<"    ";
                            f->second.pop();
                        }
                        cout<<endl;
                    }
                    cout<<"\n\n\n";*/
                   // cout<<"\n\n\n";
                    pos0.clear();
                }
            }
            if(line[line.size()-1]=='/' && open==2){
                open=0;
            }
           // cout<<line;
            if(i>0){
                //cout<<"inside i>0";
                s += (" "+line);
            }
            else
                s+=line;
           // cout<<s<<"1 ";
            i++;
            int x=firstsemioccur(line);
            //cout<< "x::"<<x<<endl;
            if(x!=-1){
                //cout<<"coco\n";
                s=removeextraspace(s);
                ans+=s;
               // cout<<s<<endl;
                s="";
                i=0;
            }
           // if(line!="")
              //  query.push_back(line);
            
        }
        /*for(int l=0;l<pos.size();l++){
            for(auto k=pos[l].begin();k!=pos[l].end();k++){
                cout<<k->first<<"   ";

                while(!k->second.empty()){
                    cout<<k->second.front().first<<" "<<k->second.front().second<<"      ";
                    k->second.pop();
                
                }
                cout<<endl;
            }
            cout<<endl;
        }*/
        // Close the file
        input_file.close();
    }
    query.push_back(ans);
    remove_multiline_comment_from_middle(query);
  //  remove_multiline_comment_from_start(query);
  //  remove_multiline_comment_from_end(query);
    vector<string> result;
    string temp;
    for(int i=0;i<query[0].size();i++){
       // cout<<query[0][i];
        temp.push_back(query[0][i]);
        if(query[0][i]==';')   {
            result.push_back(temp);
            temp="";
        }
    }
    for(int i=0;i<result.size();i++){
        cout<<"For Query "<<(i+1)<<":\n";
        generateTokens(myDFA,result[i],pos[i],symbol_table);
        myParser->parse();
        cout<<"hello"<<endl;
     //   cout<<result[i]<<endl;
    }
    ofstream fout;
    string line;
    fout.open("SymbolTable.txt");
    while (fout) {
        fout<<"The symbol table is:\n";
        for(auto k=symbol_table.begin();k!=symbol_table.end();k++){
            fout<<k->first<<"   ";
            int space=0;
            while(!k->second.empty()){
                if(space>0){
                    int r=(k->first).size();
                    r+=3;
                    while(r--){
                        fout<<" ";
                    }
                }
                fout<<k->second.top().first<<"  "<<k->second.top().second.first<<" "<<k->second.top().second.second<<endl;
                k->second.pop();
                space++;
            }
            fout<<"\n";
        }
        break;
    }
    fout.close();


    //token extraction and identification
   /* for(int i=0;i<query.size();i++)
    {
        generateTokens(myDFA,query[i]);
        //cout<< query[i]<<endl;
    }*/
    //parsing
    return 0;
}
