#include <iostream>
#include <fstream>
#include "grammar.h"

using namespace std;

int main() {
    Grammar G("data.in");
    string word;

    ifstream fin("word.in");
    fin >> word;
    fin.close();

    bool inGrammar = false;
    G.isWordInGrammar(word, 0, G.getStartSymbol(), inGrammar);

    cout << (inGrammar ? "Word accepted!\n" : "Word rejected!\n");

    return 0;
}