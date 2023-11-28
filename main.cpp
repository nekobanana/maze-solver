#include <iostream>
#include "Maze.h"
#include <opencv2/opencv.hpp>
#include <filesystem>
#include <omp.h>

#ifdef _OPENMP
#define N_PARTICLES 100
#define getThreadNumber() omp_get_thread_num()
#define N_THREADS 8
#else
#define N_PARTICLES 1
#define getThreadNumber() (-1)
#endif

#define N_MAZES 7
#define N_RUNS 100
#define SAVE_SOLUTION_IMG false

int main() {
    #ifdef _OPENMP
        std::cout << "_OPENMP defined" << std::endl;
    #endif
    cv::RNG rng( 0 );
    std::filesystem::path basePath = std::filesystem::current_path();
    double mazes_total_sum_run_time = 0;
    for (int maze_index = 0; maze_index < N_MAZES; maze_index++) {
        std::filesystem::path imgPath = std::filesystem::relative("images/" + std::to_string(maze_index) + ".png");
        Maze maze;
        maze.load_maze_from_image((basePath / imgPath).string());
        if (!maze.isStartSet())
            return -1;
        Cell startCell = maze.getStartCell();
        double maze_sum_run_time = 0;
        std::cout <<std::endl << "Maze " << maze_index << ", "
        << maze.getCellsPerRow() << " x " << maze.getCellsPerCol() << std::endl;
        for (int run = 0; run < N_RUNS; run++) {
            bool solution_found = false;
            bool solution_found_lock = false;
//          double startTime = omp_get_wtime();
            auto startTime = std::chrono::high_resolution_clock::now();
            #pragma omp parallel for num_threads(N_THREADS) default(none), firstprivate(startCell, rng), \
            shared(solution_found, solution_found_lock, maze, std::cout, startTime, run, maze_sum_run_time)
            for (int i = 0; i < N_PARTICLES; i++) {
//              double endTime = 0;
                auto endTime = startTime;
                Cell cell = startCell;
                std::vector<std::pair<int, int>> path;
                bool out = false;
                while (!out && !solution_found) {
                    path.push_back(std::pair(cell.getX(), cell.getY()));
                    int random = rng.uniform(0, cell.getPossibleDirectionsCount());
                    try {
                        cell = maze.move(cell, cell.getDirectionFromIndex(random));
                    }
                    catch (OutOfMazeException &e) {
                        out = true;
                        solution_found = true;
                        bool has_lock = false;
//                      endTime = omp_get_wtime();
                        #pragma omp critical
                        {
                            if (!solution_found_lock) {
                                solution_found_lock = true;
                                has_lock = true;
                            }
                        }
                        if (has_lock) {
                            endTime = std::chrono::high_resolution_clock::now();
                            std::cout << "Run " << run << ", Thread " << getThreadNumber() << ", particle " << i;
//                    std::cout << "\t " << endTime - startTime << "s" << std::endl;
                            double s = (double)std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count() / 1000000000;
                            std::cout << "\t " << s << "s" << std::endl;
                            maze_sum_run_time += s;
                            if (SAVE_SOLUTION_IMG && run == N_RUNS - 1) {
                                maze.save_solution_image(path);
                            }
                        }
                    }
                }
            }
        }
        std::cout << "Maze " << maze_index << " average run time:" << maze_sum_run_time / N_RUNS << std::endl;
        mazes_total_sum_run_time += maze_sum_run_time / N_RUNS;
    }
    std::cout << std::endl << "Mazes total average run time:" << mazes_total_sum_run_time / N_MAZES << std::endl;
    return 0;
}
