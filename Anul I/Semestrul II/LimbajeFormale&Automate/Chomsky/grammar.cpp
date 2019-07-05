//
// Created by Dany on 24.04.2019.
//

#include <iostream>
#include <fstream>
#include "grammar.h"

/**
 *  Constructor for class Grammar
 *  Initialize the grammar with values from a given file
 * @param inputFilePath - string for the path of the input file
 */
Grammar::Grammar(const string &inputFilePath) {
    ifstream fin(inputFilePath);
    if (!fin) {
        cout << "Could not open input file!\n";
        exit(EXIT_FAILURE);
    }

    int cnt;
    fin >> cnt;
    for (int i = 0; i < cnt; ++i) {
        string nonterm;
        fin >> nonterm;
        nonterminals_.push_back(nonterm);
    }

    fin >> cnt;
    for (int i = 0; i < cnt; ++i) {
        char term;
        fin >> term;
        terminals_.push_back(term);
    }

    fin >> startSymbol_;

    fin >> cnt;
    for (int i = 0; i < cnt; ++i) {
        string nonterm;
        string prod;
        fin >> nonterm >> prod;
        productions_[nonterm].push_back(prod);
    }

    fin.close();
}

/**
 *  Count the number of nonterminals in a given production
 * @param production - the given production
 * @return the number of nonterminals
 */
int Grammar::cntNonterminalsInProduction(const string &production) const {
    int cnt = 0;
    for (const auto &nonterminal : nonterminals_) {
        string::size_type pos = 0;
        while ((pos = production.find(nonterminal, pos)) != string::npos) {
            ++cnt;
            pos += nonterminal.length();
        }
    }
    return cnt;
}

/**
 *  Count the number of terminals in a given production
 * @param production - the given production
 * @return the number of terminals
 */
int Grammar::cntTerminalsInProduction(const string &production) const {
    int cnt = 0;
    for (const auto &terminal : terminals_)
        if (production.find(terminal) != string::npos)
            ++cnt;
    return cnt;
}

/**
 *  If there is a production which has the start symbol in the right part, add a new
 *  starting symbol, S0
 */
void Grammar::addNewStartSymbol() {
    bool found = false;
    for (const auto &nonterminal : nonterminals_)
        for (const auto &production : productions_[nonterminal])
            if (production.find(startSymbol_) != string::npos)
                found = true;

    if (found) {
        nonterminals_.emplace_back("S0");
        productions_["S0"].push_back(startSymbol_);
        startSymbol_ = "S0";
    }
}

/**
 *  Eliminate all the productions that have lambda in the right member
 */
void Grammar::eliminateLambdaProductions() {
    for (const auto &nonterminal : nonterminals_) {
        auto itFindLambdaProd = find(productions_[nonterminal].begin(), productions_[nonterminal].end(), ".");
        if (itFindLambdaProd != productions_[nonterminal].end()) {
            productions_[nonterminal].erase(itFindLambdaProd);
            for (const auto &nonterm : nonterminals_)
                for (auto &prod : productions_[nonterm]) {
                    auto itFindNonterm = prod.find(nonterminal);
                    if (itFindNonterm != string::npos) {
                        string newProduction = prod;
                        newProduction.erase(itFindNonterm, 1);

                        if (!productions_[nonterminal].empty()) {
                            if (!newProduction.empty())
                                productions_[nonterm].push_back(newProduction);
                        } else {
                            prod = newProduction;
                            auto itFindInNonterminals = find(nonterminals_.begin(), nonterminals_.end(), nonterminal);
                            if (itFindInNonterminals != nonterminals_.end())
                                nonterminals_.erase(find(nonterminals_.begin(), nonterminals_.end(), nonterminal));
                        }
                    }
                }
        }
    }
}

/**
 *  Eliminate the renames
 */
void Grammar::eliminateRenames() {
    bool done = false;
    while (!done) {
        done = true;
        for (const auto &nonterminal : nonterminals_) {
            vector<string> tempProds;
            for (auto productionNonterm = productions_[nonterminal].begin();
                 productionNonterm != productions_[nonterminal].end(); ++productionNonterm) {
                if (cntNonterminalsInProduction(*productionNonterm) == 1 &&
                    cntTerminalsInProduction(*productionNonterm) == 0) {
                    done = false;

                    for (const auto &prod : productions_[*productionNonterm])
                        tempProds.push_back(prod);

                    string product = *productionNonterm;
                    productions_[nonterminal].erase(productionNonterm);

                    bool found = false;
                    for (const auto &nonterm : nonterminals_)
                        for (const auto &prod : productions_[nonterm])
                            if (prod.find(product) != string::npos)
                                found = true;
                    if (!found) {
                        nonterminals_.erase(find(nonterminals_.begin(), nonterminals_.end(), product));
                        productions_[product].erase(productions_[product].begin(), productions_[product].end());
                    }

                    --productionNonterm;

                } else
                    tempProds.push_back(*productionNonterm);
            }
            productions_[nonterminal] = tempProds;
        }
    }
}

/**
 *  Replace all the terminals in the right member of the productions with terminals with the form Xx
 */
void Grammar::eliminateTerminals() {
    unordered_map<char, string> symbolToNonterminal;
    int cnt = 1;
    bool done = false;
    while (!done) {
        done = true;
        for (const auto &nonterminal : nonterminals_) {
            for (auto &production : productions_[nonterminal])
                if (cntTerminalsInProduction(production) && cntNonterminalsInProduction(production)) {
                    done = false;
                    string newProduction;
                    for (auto letter : production)
                        if (find(terminals_.begin(), terminals_.end(), letter) != terminals_.end()) {
                            if (symbolToNonterminal.find(letter) == symbolToNonterminal.end()) {
                                string temp("X");
                                temp.push_back(cnt++ + '0');
                                symbolToNonterminal[letter] = temp;
                                nonterminals_.push_back(temp);

                                string prod;
                                prod.push_back(letter);
                                productions_[temp].push_back(prod);
                            }
                            newProduction += symbolToNonterminal.find(letter)->second;

                        } else
                            newProduction.push_back(letter);
                    production = newProduction;
                }
        }
    }
}

/**
 *  Break the productions with 3 or more nonterminals in the right member in productions with 2
 *  nonterminals
 */
void Grammar::addNonterminalsLongProductions() {
    unordered_map<string, string> prodToNonterm;
    int cnt = 1;
    bool done = false;
    while (!done) {
        done = true;
        for (const auto &nonterminal : nonterminals_)
            for (auto &production : productions_[nonterminal])
                if (cntNonterminalsInProduction(production) > 2) {
                    done = false;

                    string prefix = production;
                    string suffix = production;
                    if (isdigit(*(production.begin() + 1))) {
                        prefix.erase(prefix.begin() + 2, prefix.end());
                        suffix.erase(suffix.begin(), suffix.begin() + 2);
                    } else {
                        prefix.erase(prefix.begin() + 1, prefix.end());
                        suffix.erase(suffix.begin(), suffix.begin() + 1);
                    }

                    if (prodToNonterm.find(suffix) == prodToNonterm.end()) {
                        string temp("Y");
                        temp.push_back(cnt++ + '0');
                        prodToNonterm[suffix] = temp;

                        productions_[temp].push_back(suffix);
                        nonterminals_.push_back(temp);
                    }
                    production = prefix + prodToNonterm.find(suffix)->second;
                }
    }
}

/**
 *  Transform to Chomsky Normal Form, using the previous functions
 */
void Grammar::transformToChomskyNormalForm() {
    addNewStartSymbol();
    eliminateLambdaProductions();
    eliminateRenames();
    eliminateTerminals();
    addNonterminalsLongProductions();
}

/**
 *  Print the grammar
 */
void Grammar::printGrammar() {
    cout << "There are " << nonterminals_.size() << " nonterminal symbols. They are: ";
    for (const auto &nonterm : nonterminals_)
        cout << nonterm << " ";
    cout << "\n";

    cout << "There are " << terminals_.size() << " terminal symbols. They are: ";
    for (const auto &term : terminals_)
        cout << term << " ";
    cout << "\n";

    cout << "The start symbol is " << startSymbol_ << "\n";

    int cntProd = 0;
    for (const auto &it : productions_)
        cntProd += productions_[it.first].size();
    cout << "There are " << cntProd << " productions. They are:\n";

    cntProd = 1;
    for (const auto &nonterm : nonterminals_)
        for (const auto &prod : productions_[nonterm])
            cout << cntProd++ << ". " << nonterm << " -> " << prod << "\n";

}