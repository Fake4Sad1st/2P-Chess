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

void Piece::setVal(int _val){
    if(_val == -1){
        image.freeTexture();
        val = -1;
        return;
    }
    val = _val % 6;
    side = ( _val >= 6 ? 1 : 0 );
    assert(0 <= val && val < 12);
    Piece::setImage(PieceLink[_val]);
}

void Piece::eaten(){
    image.freeTexture();
    side = 0, val = -1;
}

void Piece::draw(){
    if(!isPiece()) return;
    image.draw(XOFF + y * PIECE_SIZE, YOFF + x * PIECE_SIZE);
}

int Piece::getx(){
    return x;
}

int Piece::gety(){
    return y;
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
    cerr << "val=" << other.getVal() << '\n';
    if(other.getVal() == -1) return false;
    return side == other.getSide();
}

bool Piece::isPiece(){
    return val != -1;
}
