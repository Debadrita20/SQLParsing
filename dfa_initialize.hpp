
void initialiseDFA(dfa *myDFA){

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

    myDFA -> addTransition(2, 2, '_');
    myDFA -> addTransition(2, 2, '#');
    myDFA -> addTransition(2, 2, '$');
    myDFA -> addTransition(2, 2, 2);
    myDFA -> addTransition(2, 2, 1);


    myDFA -> addTransition(0, 3, 2);
    myDFA -> addTransition(3, 3, 2);
    myDFA -> addTransition(3, 4, '.');
    myDFA -> addTransition(4, 5, 2);
    myDFA -> addTransition(5, 5, 2);
    myDFA -> addTransition(0, 6, '\'');            
    myDFA -> addTransition(6, 7, 1);
    myDFA -> addTransition(7, 7, 1);
    myDFA -> addTransition(7, 8, '\'');            
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

}
   
