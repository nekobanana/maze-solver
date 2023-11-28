//
// Created by quacksort on 28/11/23.
//

#ifndef MAZE_SOLVER_CELL_H
#define MAZE_SOLVER_CELL_H


#include "Maze.h"

class Cell {
public:
    Cell(int x, int y, bool border_left, bool border_up, bool border_right, bool border_down);
    Direction getDirectionFromIndex(int idx);
    int getX() const;
    int getY() const;
    int getPossibleDirectionsCount() const;
    bool hasBorder(Direction direction);

private:
    int x;
    int y;
    int possibleDirectionsCount = 0;
    bool borders[4];
    Direction directionsIndex[4]{};
};


#endif //MAZE_SOLVER_CELL_H
