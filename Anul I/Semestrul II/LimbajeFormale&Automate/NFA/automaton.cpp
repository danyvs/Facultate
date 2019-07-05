//
// Created by Dany on 08.03.2019.
//

#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include "automaton.h"

using namespace std;

/**
 * Constructor for class Automaton
 * Read the input
 */
Automaton::Automaton() {
    ifstream fin("dataNFA.txt");

    // states
    fin >> cntStates;
    for (int i = 0; i < cntStates; ++i) {
        int temp;
        fin >> temp;
        states.push_back(temp);
    }

    // symbols
    fin >> cntSymbols;
    for (int i = 0; i < cntSymbols; ++i) {
        char temp;
        fin >> temp;
        symbols.push_back(temp);
    }

    // transitions
    fin >> cntTransitions;
    for (int i = 0; i < cntTransitions; ++i) {
        int stateA, stateB;
        char symbol;
        fin >> stateA >> symbol >> stateB;
        transitions[stateA].push_back(make_pair(stateB, symbol));
    }

    // initial state
    fin >> initialState;

    // finalStates
    fin >> cntFinalStates;
    for (int i = 0; i < cntFinalStates; ++i) {
        int temp;
        fin >> temp;
        finalStates.push_back(temp);
    }

    fin.close();
}

/**
 * Destructor for class Automaton
 * Free the allocated memory
 */
Automaton::~Automaton() {
    states.clear();
    symbols.clear();
    transitions.clear();
    finalStates.clear();
}

/**
 * Function isWordAccepted checks if an inputted word is accepted by the Automaton
 * We have the Automaton represented as a graph and to check if a word is accepted, we do a DFS,
 * but we add some restraints
 * @param currentState - the state in which the Automaton is at current stage
 * @param word - the word to be checked
 * @param pos - the position in word of the letter we check at current stage
 * @param done - variable to control if the word is accepted or not
 */
void Automaton::isWordAccepted(int currentState, string word, int pos, bool &done) {
    if (!done) {
        // the word is not accepted yet
        if (pos < word.size()) {
            // try to go the next state
            for (auto it : transitions[currentState])
                if (it.second == word[pos])
                    isWordAccepted(it.first, word, pos + 1, done);
        }
        else {
            // reached the end of the word => check if we are in a final state
            if (find(finalStates.begin(), finalStates.end(), currentState) != finalStates.end())
                done = true;
        }
    }
    else
        return;
}

/**
 * Getter for the initial sate of the Automaton
 * @return initial state
 */
int Automaton::getInitialState() {
    return initialState;
}