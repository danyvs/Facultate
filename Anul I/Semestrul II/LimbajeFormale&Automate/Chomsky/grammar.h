//
// Created by Dany on 24.04.2019.
//

#ifndef NORMALFORMCHOMSKY_GRAMMAR_H
#define NORMALFORMCHOMSKY_GRAMMAR_H

#include <algorithm>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

class Grammar {
    vector<string> nonterminals_;
    vector<char> terminals_;
    string startSymbol_;
    unordered_map<string, vector<string>> productions_;

    int cntNonterminalsInProduction(const string& production) const ;
    int cntTerminalsInProduction(const string& production) const;

    void addNewStartSymbol();
    void eliminateLambdaProductions();
    void eliminateRenames();
    void eliminateTerminals();
    void addNonterminalsLongProductions();
public:
    Grammar(const string& inputFilePath);
    void transformToChomskyNormalForm();
    void printGrammar();
};


#endif //NORMALFORMCHOMSKY_GRAMMAR_H
