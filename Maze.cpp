#include <iostream>
#include <vector>
#include "Maze.h"

Cell::Cell(int x, int y, bool border_left, bool border_up, bool border_right, bool border_down) :
        x(x), y(y), borders{border_left, border_up, border_right, border_down}{
    for (int i = 0; i < 4; i++) {
        directionsIndex[i] = Direction::null;
        if (!borders[i]) {
            directionsIndex[numPossibleDirections] = static_cast<Direction>(i);
            numPossibleDirections++;
        }
    }

}

Direction Cell::getDirectionFromIndex(int idx) {
    return directionsIndex[idx];
}

Cell Maze::move(Cell cell, Direction direction) {
    if (cell.borders[direction]) throw direction;
    switch (direction) {
        case left:
            return getCell(cell.x - 1, cell.y);
        case up:
            return getCell(cell.x, cell.y - 1);
        case right:
            return getCell(cell.x + 1, cell.y);
        case down:
            return getCell(cell.x, cell.y + 1);
        default:
            throw direction;
    }
}

Cell Maze::getCell(int x, int y) {
    if (x >= width || y >= height)
        throw OutOfMazeException();
    return cells[x * height + y];
}

bool Maze::isStartSet() const {
    return start_set;
}

Cell Maze::getStartCell() {
    return getCell(start_x, start_y);
}

