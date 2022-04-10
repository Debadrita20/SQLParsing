#include <bits/stdc++.h>

using namespace std;
void initialise_k(vector<string> *keywords)
{
    keywords->push_back("SELECT");
    keywords->push_back("FROM");
    keywords->push_back("WHERE");
    keywords->push_back("GROUP");
    keywords->push_back("BY");
    keywords->push_back("HAVING");
    keywords->push_back("ORDER");
    keywords->push_back("ASC");
    keywords->push_back("DESC");
    keywords->push_back("AS");
    keywords->push_back("INSERT");
    keywords->push_back("INTO");
    keywords->push_back("VALUES");
    keywords->push_back("DELETE");
}
void initialise_fn(vector<string> *fnames)
{
    fnames->push_back("SUM");
    fnames->push_back("COUNT");
    fnames->push_back("MAX");
    fnames->push_back("MIN");
    fnames->push_back("AVG");
    fnames->push_back("STDDEV");
    fnames->push_back("VARIANCE");
    //fnames->push_back("FIRST");
    //fnames->push_back("LAST");
}