cmake_minimum_required(VERSION 3.26)
project(maze_solver)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

add_executable(maze_solver main.cpp
        Maze.cpp
        Maze.h
)
