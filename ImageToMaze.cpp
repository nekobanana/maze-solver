#include <iostream>
#include <opencv2/opencv.hpp>
#include "Maze.h"

using namespace cv;

void Maze::load_maze_from_image(std::string filename) {
    // Caricare l'immagine png del labirinto
    Mat orig = imread(filename, IMREAD_GRAYSCALE);
    // Convertire l'immagine in una matrice binaria
    Mat image;
    threshold(orig, image, 127, 255, THRESH_BINARY);
    Mat profile_projection_x, profile_projection_y;
    reduce(image, profile_projection_y, 0, REDUCE_SUM);
    reduce(image, profile_projection_x, 1, REDUCE_SUM);

}
