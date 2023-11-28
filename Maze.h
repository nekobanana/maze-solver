#ifndef MAZE_SOLVER_MAZE_H
#define MAZE_SOLVER_MAZE_H

#include <vector>
#include <opencv2/core/mat.hpp>
#include <map>
#include "Cell.h"

enum Direction {left, up, right, down, null=-1};

class Maze {
public:
    Cell move(Cell cell, Direction direction);
    void load_maze_from_image(std::string filename);
    Cell getStartCell();
    bool isStartSet() const;
    Cell getCell(int x, int y);
    void save_solution_image(const std::vector<std::pair<int, int>>& points);

    int getCellsPerRow() const;

    int getCellsPerCol() const;

private:
    std::vector<Cell> cells;
    int width{0}, height{0};
    int image_width{0}, image_height{0};
    int border_size_x{0}, border_size_y{0}, cell_size_x{0}, cell_size_y{0};
    int start_x, start_y;
    bool start_set = false;
    int cells_per_row, cells_per_col;
    cv::Mat orig_image;
    std::string image_name;
    std::string output_folder = "solutions";
    void
    analyze_borders_x(const cv::Mat &image, std::vector<int> &border_sizes_x, std::vector<int> &cell_sizes_x) const;
};

class OutOfMazeException: public std::exception {};

#endif //MAZE_SOLVER_MAZE_H
