#include <iostream>
#include "Maze.h"
#include <opencv2/opencv.hpp>
#include <filesystem>


int main() {

    std::filesystem::path basePath = std::filesystem::current_path();
    std::filesystem::path imgPath = std::filesystem::relative("images/0.png");
    Maze maze;
    maze.load_maze_from_image((basePath / imgPath).string());

    return 0;
}
