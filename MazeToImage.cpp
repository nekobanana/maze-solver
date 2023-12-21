#include <iostream>
#include <opencv2/opencv.hpp>
#include <filesystem>
#include "Maze.h"

void Maze::save_solution_image(const std::list<std::pair<int, int>>& points) {
    cv::Mat image = orig_image.clone();
    cv::cvtColor(orig_image, image, cv::COLOR_GRAY2BGR);
    int thickness = 2;
    int x = start_x;
    int y = start_y;
    int prev_x = x * (cell_size_x + border_size_x) + border_size_x + cell_size_x / 2;
    int prev_y = y * (cell_size_y + border_size_y) + border_size_y + cell_size_y / 2;
    for (auto point : points) {
        x = point.first * (cell_size_x + border_size_x) + border_size_x + cell_size_x / 2;
        y = point.second * (cell_size_y + border_size_y) + border_size_y + cell_size_y / 2;
        line(image, cv::Point(prev_x, prev_y), cv::Point(x, y),
             cv::Scalar(0, 170, 255), thickness);
        prev_x = x;
        prev_y = y;
    }
    cv::imwrite((std::filesystem::path(output_folder) / image_name).string(), image);
}