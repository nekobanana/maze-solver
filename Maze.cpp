#include <iostream>
#include <vector>
#include "Maze.h"

Cell::Cell(int x, int y, bool border_left, bool border_up, bool border_right, bool border_down) :
        x(x), y(y), borders{border_left, border_up, border_right, border_down}{
    for (int i = 0; i < 4; i++) {
        if (borders[i]) {
            directionsIndex[numBorders] = static_cast<Direction>(i);;
            numBorders++;
        }
    }

}


Cell Maze::move(Cell cell, Direction direction) {
    if (cell.borders[direction]) {
        switch (direction) {
            case left:
                return getCell(cell.x - 1, cell.y);
            case up:
                return getCell(cell.x, cell.y - 1);
            case right:
                return getCell(cell.x + 1, cell.y);
            case down:
                return getCell(cell.x, cell.y + 1);
        }
    }
    throw direction;
}

Cell Maze::getCell(int x, int y) {
    return cells[x * width + y];
}

int Maze::getStartX() const {
    return start_x;
}

int Maze::getStartY() const {
    return start_y;
}
