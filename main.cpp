#include <iostream>
#include "Maze.h"
#include <opencv2/opencv.hpp>
#include <filesystem>


int main() {
    cv::RNG rng( 0 );
    std::filesystem::path basePath = std::filesystem::current_path();
    std::filesystem::path imgPath = std::filesystem::relative("images/0.png");
    Maze maze;
    maze.load_maze_from_image((basePath / imgPath).string());
    if (!maze.isStartSet())
        return -1;
    Cell cell = maze.getStartCell();
    std::vector<std::pair<int, int>> path;
    bool out = false;
    while (!out) {
        path.push_back(std::pair(cell.x, cell.y));
        int random = rng.uniform(0, cell.numPossibleDirections);
        try {
            cell = maze.move(cell, cell.getDirectionFromIndex(random));
        }
        catch (OutOfMazeException &e) {
            out = true;
        }
    }
    for (auto p : path) {
        std::cout<<"("<<p.first<<", "<<p.second<<")"<<std::endl;
    }
    return 0;
}
