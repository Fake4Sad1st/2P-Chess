#include<SDL.h>
#include<SDL_image.h>
#include <vector>

#include "chessboard.hpp"

Chessboard::Chessboard():
    chessboard(chessboardLink)
{
}

Chessboard::~Chessboard(){}

void Chessboard::initValue(){
    value = {
        {9, 8, 7, 10, 11, 7, 8, 9},
        {6, 6, 6, 6, 6, 6, 6, 6},
        {-1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {3, 2, 1, 4, 5, 1, 2, 3}
    };
}

void Chessboard::draw(){
    chessboard.draw(0, 0);
}

int Chessboard::getValue(int x, int y){
   return value[x][y];
}
