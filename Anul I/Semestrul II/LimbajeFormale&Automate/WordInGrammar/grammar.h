//
// Created by Dany on 03.04.2019.
//

#ifndef CLEANPROJECT_GRAMMAR_H
#define CLEANPROJECT_GRAMMAR_H

#include <algorithm>
#include <vector>
#include <map>
#include <string>

using namespace std;

class Grammar {
    vector<char> nonterminals;
    vector<char> terminals;
    char startSymbol;
    map<char, vector<pair<char, char>>> productions;
public:
    Grammar(string inputFilePath);
    ~Grammar();
    void isWordInGrammar(string word, int pos, char neterminal, bool& done);
    char getStartSymbol();

    // void generate(int n, char terminal, int dim, string word);
};

#endif //CLEANPROJECT_GRAMMAR_H
