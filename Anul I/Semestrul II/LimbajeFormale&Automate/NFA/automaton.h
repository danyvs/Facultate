//
// Created by Dany on 08.03.2019.
//

#include <algorithm>
#include <vector>
#include <map>

using namespace std;

#ifndef LFA_NFA_AUTOMATON_H
#define LFA_NFA_AUTOMATON_H

class Automaton {
    int cntStates;
    vector<int> states;
    int cntSymbols;
    vector<char> symbols;
    int cntTransitions;
    map<int, vector<pair<int, char>>> transitions;
    int initialState;
    int cntFinalStates;
    vector<int> finalStates;
public:
    Automaton();
    ~Automaton();
    void isWordAccepted(int currentState, string word, int pos, bool &done);
    int getInitialState();
};

#endif //LFA_NFA_AUTOMATON_H
