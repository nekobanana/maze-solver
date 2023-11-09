//
// Created by quacksort on 07/11/23.
//

#ifndef MAZE_SOLVER_MAZE_H
#define MAZE_SOLVER_MAZE_H

#include <vector>
#include <opencv2/core/mat.hpp>
#include <map>

enum Direction {left, up, right, down};

class Cell {
public:
    Cell(int x, int y, bool border_left, bool border_up, bool border_right, bool border_down);
    const int x;
    const int y;
    int numBorders = 0;
    const bool borders[4];
private:
    Direction directionsIndex[4]{};
};

class Maze {
public:
    Cell move(Cell cell, Direction direction);
    void load_maze_from_image(std::string filename);

    int getStartX() const;

    int getStartY() const;

private:
    std::vector<Cell> cells;
    int width{0}, height{0};
    int start_x, start_y;
    bool start_set = false;
    Cell getCell(int x, int y);

    void
    analyze_borders_x(const cv::Mat &image, std::vector<int> &border_sizes_x, std::vector<int> &cell_sizes_x) const;

    Cell* getStartCell();
};

#endif //MAZE_SOLVER_MAZE_H
