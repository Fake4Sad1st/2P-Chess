#include <string>
#include <SDL.h>
#include <SDL_image.h>

#include "piece.hpp"
#include "chessboard.hpp"

Piece::Piece(){}
Piece::~Piece(){}

void Piece::setImage(const std::string& path){
    image = Texture(path);
}

void Piece::setPos(int _x, int _y){
    x = _x;
    y = _y;
}

//void Piece::move(){
//    switch(val){
//        case PAWN:
//            if(side == BLACK){
//                assert
//            }
//    }
//}

void Piece::setVal(int _val){
    if(_val == -1){
        image.freeTexture();
        val = -1;
        return;
    }
    val = _val % 6;
    side = ( _val >= 6 ? 1 : 0 );
    Piece::setImage(PieceLink[_val]);
}

void Piece::eaten(){
    image.freeTexture();
    x = y = val = -1;
}

void Piece::draw(){
    if(val == -1) return;
    image.draw(XOFF + y * PIECE_SIZE, YOFF + x * PIECE_SIZE);
}

int Piece::getx(){
    return x;
}

int Piece::gety(){
    return y;
}

