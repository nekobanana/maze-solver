#include <iostream>
#include "Maze.h"
#include <opencv2/opencv.hpp>
#include <filesystem>


int main() {

    std::filesystem::path basePath = std::filesystem::path("/home/quacksort/CLionProjects/maze_solver");
    std::filesystem::path imgPath = std::filesystem::relative("images/0.png");
    Maze maze;
    std::cout<<"aaaaaaaaaaaaaaa"<<std::endl;
    std::flush(std::cout);
    maze.load_maze_from_image(absolute((basePath / imgPath)));

    return 0;
}
