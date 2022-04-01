
#include <bits/stdc++.h>
#include "dfa.hpp"
#include "tokengenerator.cpp"

using namespace std;
  
int main(int argc, char** argv)
{
    vector<string> query;
    dfa *myDFA=new dfa();
    //initialise myDFA
    
    if(argc==1){
        cout<< "Enter no of the query" << endl;
        int count;
        cin >> count;
        for(int i=0;i<count;i++){
            //cout <<"hii";
            string s;
            getchar();
            getline(cin, s);
            int found = s.find("--");
            if (found != string::npos){
                s.erase(s.begin() + found, s.end());
            }
            query.push_back(s);
		}
        //cout << query[0];
    }
    else if(argc==2){
        // Creation of ifstream class object to read the file
        ifstream fin;
        string line;
        // by default open mode = ios::in mode
        fin.open(argv[1]);
       
        
        
        // Execute a loop until EOF (End of File)
        while (fin) {
    
            // Read a Line from File
            getline(fin, line);
            //cout<<line;
            int found = line.find("--");
            if (found != string::npos){
                line.erase(line.begin() + found, line.end());
            }
            if(line!="")
                query.push_back(line);
            //cout<<line<<endl;
        }
        
        // Close the file
        fin.close();
    }
    for(int i=0;i<query.size();i++)
    {
        generateTokens(myDFA,query[i]);
    }    
    //parsing
    return 0;
}
