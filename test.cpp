
#include <bits/stdc++.h>
#include "dfa.hpp"
#include "tokengenerator.cpp"

using namespace std;
  
int main(int argc, char** argv)
{
    vector<string> query;
    dfa *myDFA=new dfa();
    //initialise myDFA

    vector <int> finalStates = {1, 2, 3, 5, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};
    vector <int> nonFinalStates = {0, 4, 6, 7};

    int totStates = finalStates.size() + nonFinalStates.size();

    for (int i = 0; i < totStates; i++){
        myDFA->addState(0, "error");
    }

    for (int i = 0; i < finalStates.size(); i++){
        myDFA-> getState(finalStates[i])->setFinal();
    }

    myDFA-> addTransition(0, 1, 1);
    myDFA -> addTransition(1, 2, '_');
    myDFA -> addTransition(1, 2, '#');
    myDFA -> addTransition(1, 2, '$');
    myDFA -> addTransition(1, 2, 2);
    myDFA -> addTransition(1, 2, 1);
    myDFA -> addTransition(0, 3, 2);
    myDFA -> addTransition(3, 3, 2);
    myDFA -> addTransition(3, 4, '.');
    myDFA -> addTransition(4, 5, 2);
    myDFA -> addTransition(5, 5, 2);
    myDFA -> addTransition(0, 6, 39);            // 39 -> ASCII for '
    myDFA -> addTransition(6, 7, 1);
    myDFA -> addTransition(7, 7, 1);
    myDFA -> addTransition(7, 8, 39);            // 39 -> ASCII for ' 
    myDFA -> addTransition(0, 9, '<');
    myDFA -> addTransition(9, 11, '=');
    myDFA -> addTransition(0, 10, '>');
    myDFA -> addTransition(10, 11, '=');
    myDFA -> addTransition(0, 11, '=');
    myDFA -> addTransition(9, 17, '>');
    myDFA -> addTransition(0, 12, '*');
    myDFA -> addTransition(0, 13, ',');
    myDFA -> addTransition(0, 14, '(');
    myDFA -> addTransition(0, 15, ')');
    myDFA -> addTransition(0, 16, ';');


    // Set token class for states

    myDFA->getState(1)->setTokenClass("id");
    myDFA->getState(2)->setTokenClass("id");
    myDFA->getState(8)->setTokenClass("string const");
    myDFA->getState(3)->setTokenClass("integer const");
    myDFA->getState(5)->setTokenClass("float const");
    myDFA->getState(9)->setTokenClass("relational operator");
    myDFA->getState(10)->setTokenClass("relational operator");
    myDFA->getState(11)->setTokenClass("relational operator");
    myDFA->getState(17)->setTokenClass("relational operator");
    myDFA->getState(12)->setTokenClass("*");
    myDFA->getState(13)->setTokenClass(",");
    myDFA->getState(14)->setTokenClass("(");
    myDFA->getState(15)->setTokenClass(")");
    myDFA->getState(16)->setTokenClass(";");

    
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
