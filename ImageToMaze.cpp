#include <iostream>
#include <opencv2/opencv.hpp>
#include <filesystem>
#include "Maze.h"

using namespace cv;

void Maze::load_maze_from_image(std::string filename) {
    image_name = std::filesystem::path(filename).filename();
    orig_image = imread(filename, IMREAD_GRAYSCALE);
    image_width = orig_image.rows;
    image_height = orig_image.cols;
    Mat image;
    threshold(orig_image, image, 127, 255, THRESH_BINARY);

    std::vector<int> border_sizes_x, cell_sizes_x, border_sizes_y, cell_sizes_y;
    analyze_borders_x(image, border_sizes_x, cell_sizes_x);
    analyze_borders_x(image.t(), border_sizes_y, cell_sizes_y);
    border_size_x = *std::min_element(border_sizes_x.begin(), border_sizes_x.end());
    border_size_y = *std::min_element(border_sizes_y.begin(), border_sizes_y.end());
    cell_size_x = *std::min_element(cell_sizes_x.begin(), cell_sizes_x.end());
    cell_size_y = *std::min_element(cell_sizes_y.begin(), cell_sizes_y.end());
    int cells_per_row = (image_height - border_size_x) / (cell_size_x + border_size_x);
    int cells_per_col = (image_width - border_size_y) / (cell_size_y + border_size_y);
    width = cells_per_row;
    height = cells_per_col;
    for (int x = 0; x < cells_per_row; x++) {
        for (int y = 0; y < cells_per_col; y++) {
            int cell_center_x = x * (cell_size_x + border_size_x) + border_size_x + cell_size_x / 2;
            int cell_center_y = y * (cell_size_y + border_size_y) + border_size_y + cell_size_y / 2;
//            assert(image.data[image.step * cell_center_y + cell_center_x] == 255);
            bool border_up = image.data[image.step * (cell_center_y - ((border_size_y + cell_size_y) / 2)) + cell_center_x] == 0;
            bool border_left = image.data[image.step * cell_center_y + cell_center_x - ((border_size_x + cell_size_x) / 2)] == 0;
            bool border_down = image.data[image.step * (cell_center_y + ((border_size_y + cell_size_y) / 2)) + cell_center_x] == 0;
            bool border_right = image.data[image.step * cell_center_y + cell_center_x + ((border_size_x + cell_size_x) / 2)] == 0;
            this->cells.push_back(Cell(x, y, border_left, border_up, border_right, border_down));
        }
    }
    //find a start cell
    this->start_set = false;
    Direction start_cell_entrance_direction;
    for (int x = 0; x < this->width; x++) {
        if (!getCell(x, 0).hasBorder(Direction::up)) {
            this->start_x = x;
            this->start_y = 0;
            start_set = true;
            start_cell_entrance_direction = Direction::up;
            break;
        }
    }
    if (!start_set) {
        for (int y = 0; y < this->height; y++) {
            if (!getCell(0, y).hasBorder(Direction::left)) {
                this->start_x = 0;
                this->start_y = y;
                start_set = true;
                start_cell_entrance_direction = Direction::left;
                break;
            }
        }
    }
    if (!start_set) {
        for (int x = 0; x < this->width; x++) {
            if (!getCell(x, this->height - 1).hasBorder(Direction::down)) {
                this->start_x = x;
                this->start_y = this->height - 1;
                start_set = true;
                start_cell_entrance_direction = Direction::down;
                break;
            }
        }
    }
    if (!start_set) {
        for (int y = 0; y < this->height; y++) {
            if (!getCell(this->width - 1, y).hasBorder(Direction::right)) {
                this->start_x = this->width - 1;
                this->start_y = y;
                start_set = true;
                start_cell_entrance_direction = Direction::right;
                break;
            }
        }
    }
    Cell start = getCell(start_x, start_y);
    cells[start_x * height + start_y] = Cell(start.getX(), start.getY(),
                                            start.hasBorder(Direction::left) || (start_cell_entrance_direction == Direction::left),
                                            start.hasBorder(Direction::up) || (start_cell_entrance_direction == Direction::up),
                                            start.hasBorder(Direction::right) || (start_cell_entrance_direction == Direction::right),
                                            start.hasBorder(Direction::down) || (start_cell_entrance_direction == Direction::down));
}

void Maze::analyze_borders_x(const Mat &image, std::vector<int> &border_sizes_x, std::vector<int> &cell_sizes_x) const {
    for (int r = 0; r < image.rows; r++) {
        int size_counter = 0;
        bool is_border = true;
        for (int c = 0; c < image.cols; c++) {
            uchar value = image.data[image.step * r + c];
            if (value == 0 != is_border) {
                if (size_counter != 0) {
                    if (is_border) border_sizes_x.push_back(size_counter);
                    else cell_sizes_x.push_back(size_counter);
                }
                is_border = !is_border;
                size_counter = 1;
            } else {
                size_counter++;
            }
        }
        if (size_counter != 0) {
            if (is_border) border_sizes_x.push_back(size_counter);
            else cell_sizes_x.push_back(size_counter);
        }
    }
}
