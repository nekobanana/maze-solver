#include <iostream>
#include "Maze.h"
#include <opencv2/opencv.hpp>
#include <filesystem>
#include <omp.h>

#define N_PARTICLES 100

std::string pathToString(std::vector<std::pair<int, int>> &path);

int main() {
    #ifdef _OPENMP
        std::cout << "_OPENMP defined" << std::endl;
    #endif
    cv::RNG rng( 0 );
    std::filesystem::path basePath = std::filesystem::current_path();
    std::filesystem::path imgPath = std::filesystem::relative("images/5.png");
    Maze maze;
    maze.load_maze_from_image((basePath / imgPath).string());
    if (!maze.isStartSet())
        return -1;
    Cell startCell = maze.getStartCell();
    bool solution_found = false;
    #pragma omp parallel for default(none), firstprivate(startCell, rng), shared(solution_found, maze, std::cout)
    for (int i = 0; i < N_PARTICLES; i++) {
        Cell cell = startCell;
        std::vector<std::pair<int, int>> path;
        bool out = false;
        while (!out && !solution_found) {
            path.push_back(std::pair(cell.x, cell.y));
            int random = rng.uniform(0, cell.numPossibleDirections);
            try {
                cell = maze.move(cell, cell.getDirectionFromIndex(random));
            }
            catch (OutOfMazeException &e) {
                out = true;
                solution_found = true;
            }
        }
        if (out) {
            #ifdef _OPENMP
            std::cout << "Thread " << omp_get_thread_num() << ", particle " << i << std::endl;
            #else
            std::cout << "Particle " << i << std::endl;
            #endif
            maze.save_solution_image(path);
        }
    }
    return 0;
}

std::string pathToString(std::vector<std::pair<int, int>> &path) {
    std::string s;
    for (auto p: path) {
        s.append("(" + std::to_string(p.first) + ", " + std::to_string(p.second) + "), ");
    }
    return s;
}
