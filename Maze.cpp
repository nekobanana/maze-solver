#include "Maze.h"
#include "Cell.h"

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

