//
// Created by Dany on 13.04.2019.
//

#ifndef OOP_TREASURE_HUNT_ADVENTURER_H
#define OOP_TREASURE_HUNT_ADVENTURER_H

#include <algorithm>
#include <string>
#include "map.h"

using namespace std;

class Adventurer {
    const string name_;
    bool foundATreasure_;
protected:
    pair<int, int> position_;
public:
    Adventurer(const int& line, const int& column, string name);
    virtual ~Adventurer();
    virtual void move(Map& map, const int& step) = 0;
    virtual bool canAdventurerMove(Map& map) = 0;
    string getName() const;
    pair<int, int> getPosition() const;
    void setFoundATreasure(const bool& value);
    bool getFoundATreasure() const;
};

class AdventurerA : public Adventurer {
    const int dx[8] = {-1, -1, -1, 0, 1, 1, 1, 0};
    const int dy[8] = {-1, 0, 1, 1, 1, 0, -1, -1};
public:
    AdventurerA(const int& line, const int& column, const string& name);
    ~AdventurerA() override;
    bool canAdventurerMove(Map& map) override;
    void move(Map& map, const int& step) override;
};

class AdventurerB : public Adventurer {
    const int dx[4] = {-1, 0, 1, 0};
    const int dy[4] = {0, 1, 0, -1};
public:
    AdventurerB(const int& line, const int& column, const string& name);
    ~AdventurerB() override;
    bool canAdventurerMove(Map& map) override;
    void move(Map& map, const int& step) override;
};

class AdventurerC : public Adventurer {
    const int dx[4] = {-1, -1, 1, 1};
    const int dy[4] = {-1, 1, -1, 1};
public:
    AdventurerC(const int& line, const int& column, const string& name);
    ~AdventurerC() override;
    bool canAdventurerMove(Map& map) override;
    void move(Map& map, const int& step) override;
};

class AdventurerD : public Adventurer {
    const int dx[5] = {0, 1, 1, 1, 0};
    const int dy[5] = {-1, -1, 0, 1, 1};
public:
    AdventurerD(const int& line, const int& column, const string& name);
    ~AdventurerD() override;
    bool canAdventurerMove(Map& map) override;
    void move(Map& map, const int& step) override;
};

#endif //OOP_TREASURE_HUNT_ADVENTURER_H
