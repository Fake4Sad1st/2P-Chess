#include "piece.hpp"

Piece::Piece(){}
Piece::~Piece(){}

void Piece::setImage(const std::string& path){
    image = Texture(path);
}

void Piece::setVal(bool chessKind, int _val){
    if(_val == -1){
        image.free();
        val = -1;
        return;
    }
    assert(0 <= _val && _val < 12);
    val = _val % 6;
    side = ( _val >= 6 ? 1 : 0 );
    Piece::setImage(PieceLink[chessKind][_val]);
}

void Piece::eaten(){
    image.free();
    side = 0, val = -1;
}

void Piece::draw(SDL_Rect& rect){
    if(!isPiece()) return;
    image.draw(rect);
}


int Piece::getVal(){
    if(!isPiece()) return -1;
    return val;
}

int Piece::getSide(){
    if(!isPiece()) return -1;
    return side;
}

bool Piece::ally(Piece& other){
    if(!other.isPiece()) return false;
    return side == other.getSide();
}

bool Piece::enemy(Piece& other){
    if(!other.isPiece()) return false;
    return side != other.getSide();
}

bool Piece::isPiece(){
    return val != -1;
}
