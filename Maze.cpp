#include <iostream>
#include <vector>
#include "Maze.h"

Cell::Cell(int x, int y, bool border_left, bool border_up, bool border_right, bool border_down) :
        x(x), y(y), borders{border_left, border_up, border_right, border_down}{
    for (int i = 0; i < 4; i++) {
        directionsIndex[i] = Direction::null;
        if (!borders[i]) {
            directionsIndex[possibleDirectionsCount] = static_cast<Direction>(i);
            possibleDirectionsCount++;
        }
    }

}

int Cell::getPossibleDirectionsCount() const {
    return possibleDirectionsCount;
}

int Cell::getX() const {
    return x;
}

int Cell::getY() const {
    return y;
}
Direction Cell::getDirectionFromIndex(int idx) {
    return directionsIndex[idx];
}

bool Cell::hasBorder(Direction direction) {
    return borders[direction];
}

Cell Maze::move(Cell cell, Direction direction) {
    if (cell.hasBorder(direction)) throw direction;
    switch (direction) {
        case left:
            return getCell(cell.getX() - 1, cell.getY());
        case up:
            return getCell(cell.getX(), cell.getY() - 1);
        case right:
            return getCell(cell.getX() + 1, cell.getY());
        case down:
            return getCell(cell.getX(), cell.getY() + 1);
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

int Maze::getCellsPerRow() const {
    return cells_per_row;
}

int Maze::getCellsPerCol() const {
    return cells_per_col;
}
