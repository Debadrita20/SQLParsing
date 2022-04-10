
#include <bits/stdc++.h>
#include "dfa.hpp"
#include "tokengenerator.cpp"
#include "dfa_initialize.hpp"

using namespace std;
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

    //initialise myDFA
    initialiseDFA(myDFA);

    
    if(argc==1){
        
        
            //cout <<"hii";

            string input,s;
            char c,prev;
            int i=0;
            while (1)
            {
                getline(cin,input);

                
                //cout<< input<<endl;
                
                int found = input.find("--");
                if (found != string::npos){
                    input.erase(input.begin() + found, input.end());
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

        
        int i=0;
        string s;
        // Execute a loop until EOF (End of File)
        int cx=0;
        while (getline(input_file, line)) {
            cx++;
            // Read a Line from File
          //  getline(fin, line);
           // cout<<line<<endl;
            int found = line.find("--");
            if (found != string::npos){
                line.erase(line.begin() + found, line.end());
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
        generateTokens(myDFA,result[i]);
        cout<<result[i]<<endl;
    }
    //token extraction and identification
   /* for(int i=0;i<query.size();i++)
    {
        generateTokens(myDFA,query[i]);
        //cout<< query[i]<<endl;
    }
    //parsing
    return 0;
}
