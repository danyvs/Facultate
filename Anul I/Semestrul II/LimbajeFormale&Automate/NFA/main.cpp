#include <iostream>
#include <fstream>
#include "automaton.h"

using namespace std;

int main() {
    Automaton NFA;
    string word;

    ifstream fin("word.txt");
    fin >> word;
    fin.close();

    bool ok = false;
    NFA.isWordAccepted(NFA.getInitialState(), word, 0, ok);
    if (ok)
        cout << "Word accepted!\n";
    else
        cout << "Word rejected!\n";
    return 0;
}