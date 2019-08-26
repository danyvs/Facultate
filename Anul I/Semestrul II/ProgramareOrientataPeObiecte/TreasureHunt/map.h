//
// Created by Dany on 13.04.2019.
//

#ifndef OOP_TREASURE_HUNT_MAP_H
#define OOP_TREASURE_HUNT_MAP_H

#include <iostream>

class Map {
    const int cntLines_, cntColumns_;
    int** content_;
public:
    Map(const int& lines, const int& columns);
    ~Map();

    int getCntLines() const;
    int getCntColumns() const;
    int getContentPosition(const int& line, const int& column) const;
    void setContentPosition(const int& line, const int& column, const int& data);

    void printMap();
};

#endif //OOP_TREASURE_HUNT_MAP_H
