//
// Created by Dany on 03.04.2019.
//

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include "grammar.h"

using namespace std;

/**
 *  Constructor for class Grammar
 *  Read the input
 * @param inputFilePath - string, name of the input file
 */
Grammar::Grammar(string inputFilePath) {
    ifstream fin(inputFilePath);

    int cnt;

    // read nonterminals
    fin >> cnt;
    for (int i = 0; i < cnt; ++i) {
        char nonTerm;
        fin >> nonTerm;
        nonterminals.push_back(nonTerm);
    }

    // read terminals
    fin >> cnt;
    for (int i = 0; i < cnt; ++i) {
        char term;
        fin >> term;
        terminals.push_back(term);
    }

    fin >> startSymbol;

    // read productions
    fin >> cnt;
    for (int i = 0; i < cnt; ++i) {
        char term;
        string prod;
        fin >> term >> prod;
        if (prod[1] >= 'A' && prod[1] <= 'Z') {
            // the current production is terminal + nonterminal
            productions[term].push_back(make_pair(prod[1], prod[0]));
        }
        else {
            // the current production is terminal or lambda
            productions[term].push_back(make_pair('.', prod[0]));
        }
    }

    fin.close();
}

/**
 *  Destructor for class Grammar
 *  Clear memory
 */
Grammar::~Grammar() {
    nonterminals.clear();
    terminals.clear();
    productions.clear();
}

/**
 * Check if word is in grammar
 * @param word - string to be checked
 * @param pos - the position in string to be checked
 * @param terminal - current terminal
 * @param done - bool to maintain if the word is in grammar or not
 */
void Grammar::isWordInGrammar(string word, int pos, char terminal, bool& done) {
    if (!done) {
        if (pos == word.size()) {
            // end of the word

            pair<char, char> elem = make_pair('.', '.');
            if (terminal == '.' || find(productions[terminal].begin(), productions[terminal].end(), elem) != productions[terminal].end())
                done = true;
        }
        for (auto it : productions[terminal])
            if (it.second == word[pos])
                isWordInGrammar(word, pos + 1, it.first, done);
    }
}

/**
 *  getter for startSymbol
 * @return
 */
char Grammar::getStartSymbol() {
    return startSymbol;
}
/*
void Grammar::generate(int n, char terminal, int dim, string word) {
    if (word.size() <= n)
        for (auto it : productions[terminal]) {
            word[dim] = it.first;
            generate(n, it.first, dim + 1, word);
        }
    }
}*/