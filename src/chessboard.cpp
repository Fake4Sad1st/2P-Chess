#include "chessboard.hpp"

Chessboard::Chessboard():
    chessboard(ChessboardLink)
{
}

Chessboard::~Chessboard(){}

void Chessboard::initValue(){
    value = {
        {BROOK, BKNIGHT, BBISHOP, BQUEEN, BKING, BBISHOP, BKNIGHT, BROOK},
        {BPAWN, BPAWN, BPAWN, BPAWN, BPAWN, BPAWN, BPAWN, BPAWN},
        {-1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1},
        {WPAWN, WPAWN, WPAWN, WPAWN, WPAWN, WPAWN, WPAWN, WPAWN},
        {WROOK, WKNIGHT, WBISHOP, WQUEEN, WKING, WBISHOP, WKNIGHT, WROOK}
//        {-1, -1, -1, -1, -1, -1, -1, -1},
//        {-1, -1, -1, -1, -1, -1, -1, -1},
//        {-1, -1, -1, -1, -1, -1, -1, -1},
//        {-1, -1, -1, -1, -1, -1, -1, -1},
//        {-1, -1, -1, -1, -1, -1, -1, -1},
//        {-1, -1, -1, -1, -1, -1, -1, -1},
//        {-1, -1, -1, -1, -1, -1, -1, -1},
//        {-1, -1, -1, -1, -1, -1, -1, -1},

    };
}

void Chessboard::draw(){
    chessboard.draw(0, 0);
}

int Chessboard::getValue(int x, int y){
   return value[x][y];
}

void Chessboard::setValue(int x, int y, int _val){
    value[x][y] = _val;
}
