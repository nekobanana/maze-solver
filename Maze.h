#ifndef MAZE_SOLVER_MAZE_H
#define MAZE_SOLVER_MAZE_H

#include <vector>
#include <opencv2/core/mat.hpp>
#include <map>

enum Direction {left, up, right, down, null=-1};

class Cell {
public:
    Cell(int x, int y, bool border_left, bool border_up, bool border_right, bool border_down);
    int x;
    int y;
    int numPossibleDirections = 0;
    bool borders[4];
    Direction getDirectionFromIndex(int idx);
private:
    Direction directionsIndex[4]{};
};

class Maze {
public:
    Cell move(Cell cell, Direction direction);
    void load_maze_from_image(std::string filename);

    Cell getStartCell();

    bool isStartSet() const;
    Cell getCell(int x, int y);

private:
    std::vector<Cell> cells;
    int width{0}, height{0};
    int start_x, start_y;
    bool start_set = false;

    void
    analyze_borders_x(const cv::Mat &image, std::vector<int> &border_sizes_x, std::vector<int> &cell_sizes_x) const;
};

class OutOfMazeException: public std::exception {};

#endif //MAZE_SOLVER_MAZE_H
