#include "Cell.h"
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
