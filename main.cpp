#include <iostream>
#include "Maze.h"
#include <opencv2/opencv.hpp>
#include <filesystem>
#include <omp.h>
#include <fstream>
#include <random>

#ifdef _OPENMP
#define getThreadNumber() omp_get_thread_num()

#else
#define getThreadNumber() (-1)
#endif

int get_arg(const std::string& arg_name, int argc, char* argv[]) {
    std::vector<std::string> args(argv + 1, argv + argc);
    auto it = std::find(args.begin(), args.end(), "-" + arg_name);
    if (it != args.end() && ++it != args.end()) {
        return std::stoi(*it);
    }
    return 0;
}

int main(int argc, char* argv[]) {
    int n_particles = get_arg("p", argc, argv);
    int n_threads = get_arg("t", argc, argv);
    int n_runs = get_arg("r", argc, argv);
    const int save_solution = get_arg("s", argc, argv);
    if (n_threads == 0) n_threads = 1;
    if (n_runs == 0) n_runs = 100;
    std::cout << std::endl << "-p " << n_particles << " -t " << n_threads << " -r " << n_runs << "...";
    std::cout.flush();
    std::string out_file_nme = std::string("results/") + std::to_string(n_threads) +  std::string("_threads_") + \
        std::to_string(n_particles) + std::string("_p.txt");

    std::ofstream out(out_file_nme);
    std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
    std::cout.rdbuf(out.rdbuf());

    #ifdef _OPENMP
        std::cout << "_OPENMP defined" << std::endl;
        omp_lock_t solution_found_write;
        omp_init_lock(&solution_found_write);
    #endif
    std::filesystem::path basePath = std::filesystem::current_path();
    std::vector<std::filesystem::path> images;
    for (const auto & entry : std::filesystem::directory_iterator("images/")) {
        if (entry.path().filename().extension() == ".png")
            images.push_back(entry.path());
    }
    double mazes_total_sum_run_time = 0;
    for (const auto& maze_path: images) {
        std::filesystem::path imgPath = std::filesystem::relative(maze_path);
        Maze maze;
        maze.load_maze_from_image((basePath / imgPath).string());
        if (!maze.isStartSet())
            return -1;
        Cell startCell = maze.getStartCell();
        double maze_sum_run_time = 0;
        std::cout << std::endl << "Maze " << maze_path.filename() << ", "
                  << maze.getCellsPerRow() << "x" << maze.getCellsPerCol() << std::endl;
        for (int run = 0; run < n_runs; run++) {
            bool solution_found = false;
            bool solution_found_locked = false;
            auto startTime = std::chrono::high_resolution_clock::now();
            #pragma omp parallel for num_threads(n_threads) default(none), firstprivate(startCell), \
            shared(solution_found, solution_found_locked, maze, std::cout, startTime, run, maze_sum_run_time, \
            n_particles, n_runs, save_solution, solution_found_write)
            for (int i = 0; i < n_particles; i++) {
                auto endTime = startTime;
                std::random_device rd;
                std::mt19937 rng(rd());
                std::uniform_real_distribution<> dis(0, 1);
                Cell cell = startCell;
                std::list<unsigned short> particle_path_x;
                std::list<unsigned short> particle_path_y;
                auto startParticleTime = std::chrono::high_resolution_clock::now();
                while (!solution_found) {
                    if (save_solution) {
                        particle_path_x.emplace_back(cell.getX());
                        particle_path_y.emplace_back(cell.getY());
                    }
                    int random = static_cast<int>(dis(rng) * cell.getPossibleDirectionsCount());
                    try {
                        cell = maze.move(cell, cell.getDirectionFromIndex(random));
                    }
                    catch (OutOfMazeException &e) {
                        solution_found = true;
                        endTime = std::chrono::high_resolution_clock::now();
                        bool is_first;
                        #ifdef _OPENMP
                        omp_set_lock(&solution_found_write);
                        #endif
                        if (!solution_found_locked) {
                            solution_found_locked = true;
                            is_first = true;
                        }
                        #ifdef _OPENMP
                        omp_unset_lock(&solution_found_write);
                        #endif

                        if (is_first) {
                            std::cout << "Run " << run << ", Thread " << getThreadNumber() << ", particle " << i;
                            double s = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count()) / 1000000000;
                            std::cout << "\t " << s << "s" << std::endl;
                            maze_sum_run_time += s;
                            if (save_solution && run == n_runs - 1) {
                                maze.save_solution_image(particle_path_x, particle_path_y);
                            }
                        }
                    }
                }
            }
        }
        std::cout << "Maze " << maze_path.filename() << " average run time:" << maze_sum_run_time / n_runs << std::endl;
        mazes_total_sum_run_time += maze_sum_run_time / n_runs;
    }
    std::cout << std::endl << "Mazes total average run time:" << mazes_total_sum_run_time / images.size() << std::endl;
    std::cout.rdbuf(coutbuf); //reset to standard output again
    std::cout << " DONE";  //output to the standard input

    #ifdef _OPENMP
    omp_destroy_lock(&solution_found_write);
    #endif
    return 0;
}

