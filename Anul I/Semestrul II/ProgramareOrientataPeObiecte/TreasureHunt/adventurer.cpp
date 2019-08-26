//
// Created by Dany on 13.04.2019.
//

#include <algorithm>
#include "adventurer.h"

/**
 *  Constructor for class Adventurer
 *  Initialize the name and the position with given data
 * @param line - int, the line to place the Adventurer on
 * @param column - int, the column to place the Adventurer on
 * @param name - string, the name of the Adventurer
 */
Adventurer::Adventurer(const int& line, const int& column, string name) : name_(std::move(name)) {
    position_ = make_pair(line, column);
    foundATreasure_ = false;
}

/**
 *  Destructor for class Adventurer
 */
Adventurer::~Adventurer() = default;

/**
 *  Getter for the name of an Adventurer
 * @return (string) name_
 */
string Adventurer::getName() const {
    return name_;
}

/**
 *  Getter for the position of an Adventurer
 * @return (pair<int, int>) position_
 */
pair<int, int> Adventurer::getPosition() const {
    return position_;
}

/**
 *  Setter for the variable maintaining if an Adventurer found a treasure or not
 * @param value - bool
 */
void Adventurer::setFoundATreasure(const bool& value) {
    foundATreasure_ = value;
}

/**
 *  Getter for the variable maintaining if an Adventurer found a treasure or not
 * @return (bool) foundATreasure_
 */
bool Adventurer::getFoundATreasure() const {
    return foundATreasure_;
}



/**
 *  Constructor for AdventurerA
 * @param line - int, the line to place the Adventurer on
 * @param column - int, the column to place the Adventurer on
 * @param name, string, the name of the Adventurer
 */
AdventurerA::AdventurerA(const int& line, const int& column, const string& name) : Adventurer(line, column, name) {

}

/**
 *  Destructor for class AdventurerA
 */
AdventurerA::~AdventurerA() = default;

/**
 *  Check if AdventurerA can move into a neighbour cell
 * @param map - Map
 * @return true or false, if the AdventurerA can move or not
 */
bool AdventurerA::canAdventurerMove(Map &map) {
    int line = position_.first;
    int column = position_.second;
    for (int i = 0; i < 8; ++i) {
        int newLine = line + dx[i];
        int newColumn = column + dy[i];
        if (!map.getContentPosition(newLine, newColumn))
            return true;
    }

    return false;
}

/**
 *  Move function for AdventurerA
 * @param map - Map
 * @param step - the number of the current round
 */
void AdventurerA::move(Map& map, const int& step) {
    int line, column;
    int index = step % 8;
    // try to move the adventurer in a neighbour cell
    do {
        line = position_.first + dx[index];
        column = position_.second + dy[index];
        index = (index + 1) % 8;
    } while (map.getContentPosition(line, column) == 1);

    position_ = make_pair(line, column);
    map.setContentPosition(line, column, 1);
}



/**
 *  Constructor for AdventurerB
 * @param line - int, the line to place the Adventurer on
 * @param column - int, the column to place the Adventurer on
 * @param name, string, the name of the Adventurer
 */
AdventurerB::AdventurerB(const int& line, const int& column, const string& name) : Adventurer(line, column, name) {

}

/**
 *  Destructor for class AdventurerB
 */
AdventurerB::~AdventurerB() = default;

/**
 *  Check if AdventurerB can move into a neighbour cell
 * @param map - Map
 * @return true or false, if the AdventurerB can move or not
 */
bool AdventurerB::canAdventurerMove(Map &map) {
    int line = position_.first;
    int column = position_.second;
    for (int i = 0; i < 4; ++i) {
        int newLine = line + dx[i];
        int newColumn = column + dy[i];
        if (!map.getContentPosition(newLine, newColumn))
            return true;
    }

    return false;
}

/**
 *  Move function for AdventurerB
 * @param map - Map
 * @param step - the number of the current round
 */
void AdventurerB::move(Map& map, const int& step) {
    int line, column;
    int index = step % 4;
    // try to move the adventurer in a neighbour cell
    do {
        line = position_.first + dx[index];
        column = position_.second + dy[index];
        index = (index + 1) % 4;
    } while (map.getContentPosition(line, column) == 1);

    position_ = make_pair(line, column);
    map.setContentPosition(line, column, 1);
}



/**
 *  Constructor for AdventurerC
 * @param line - int, the line to place the Adventurer on
 * @param column - int, the column to place the Adventurer on
 * @param name, string, the name of the Adventurer
 */
AdventurerC::AdventurerC(const int& line, const int& column, const string& name) : Adventurer(line, column, name) {

}

/**
 *  Destructor for class AdventurerC
 */
AdventurerC::~AdventurerC() = default;

/**
 *  Check if AdventurerC can move into a neighbour cell
 * @param map - Map
 * @return true or false, if the AdventurerC can move or not
 */
bool AdventurerC::canAdventurerMove(Map &map) {
    int line = position_.first;
    int column = position_.second;
    for (int i = 0; i < 4; ++i) {
        int newLine = line + dx[i];
        int newColumn = column + dy[i];
        if (!map.getContentPosition(newLine, newColumn))
            return true;
    }

    return false;
}

/**
 *  Move function for AdventurerC
 * @param map - Map
 * @param step - the number of the current round
 */
void AdventurerC::move(Map& map, const int& step) {
    int line, column;
    int index = step % 4;
    int inc = (step % 2) ? step : step + 1;
    // try to move the adventurer in a neighbour cell
    do {
        line = position_.first + dx[index];
        column = position_.second + dy[index];
        index = (index + inc++) % 4;
    } while (map.getContentPosition(line, column) == 1);

    position_ = make_pair(line, column);
    map.setContentPosition(line, column, 1);
}



/**
*  Constructor for AdventurerD
* @param line - int, the line to place the Adventurer on
* @param column - int, the column to place the Adventurer on
* @param name, string, the name of the Adventurer
*/
AdventurerD::AdventurerD(const int& line, const int& column, const string& name) : Adventurer(line, column, name) {

}

/**
 *  Destructor for class AdventurerD
 */
AdventurerD::~AdventurerD() = default;

/**
 *  Check if AdventurerD can move into a neighbour cell
 * @param map - Map
 * @return true or false, if the AdventurerD can move or not
 */
bool AdventurerD::canAdventurerMove(Map &map) {
    int line = position_.first;
    int column = position_.second;
    for (int i = 0; i < 5; ++i) {
        int newLine = line + dx[i];
        int newColumn = column + dy[i];
        if (!map.getContentPosition(newLine, newColumn))
            return true;
    }

    return false;
}

/**
 *  Move function for AdventurerD
 * @param map - Map
 * @param step - the number of the current round
 */
void AdventurerD::move(Map& map, const int& step) {
    int line, column;
    int index = 0;
    int inc = step % 5;
    if (inc == 0)
        inc = 2;
    // try to move the adventurer in a neighbour cell
    do {
        line = position_.first + dx[index];
        column = position_.second + dy[index];
        index = (index + inc) % 5;
    } while (map.getContentPosition(line, column) == 1);

    position_ = make_pair(line, column);
    map.setContentPosition(line, column, 1);
}