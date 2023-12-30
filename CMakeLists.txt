cmake_minimum_required(VERSION 3.26)
project(maze_solver)
#set(OPENMP true)
set(OPENMP false)

set(CMAKE_CXX_STANDARD 17)

message("${CMAKE_MODULE_PATH}")

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
#set(DCMAKE_BUILD_TYPE Debug)
set(DCMAKE_BUILD_TYPE Release)
#set(DCMAKE_CXX_FLAGS_DEBUG "-g -O0")

if (OPENMP)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
endif ()

add_executable(maze_solver main.cpp
        Maze.cpp
        Maze.h
        ImageToMaze.cpp
        MazeToImage.cpp
)
target_link_libraries( maze_solver ${OpenCV_LIBS} )

if (OPENMP)
    set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "maze_solver")
else()
    set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "maze_solver_single_thread")
endif()

