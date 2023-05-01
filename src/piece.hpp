#pragma once

#include "texture.hpp"

//Piece size
const int PIECE_SIZE = 80;

enum SIDE{
    WHITE,
    BLACK,
};

enum CHESS_PIECE{
    PAWN,
    BISHOP,
    ROOK,
    KNIGHT,
    QUEEN,
    KING,
    NUM_OF_PIECES,
};

//image links
const std::string PieceLink[] = {
    "img/piece/WhitePawn.png",
    "img/piece/WhiteBishop.png",
    "img/piece/WhiteRook.png",
    "img/piece/WhiteKnight.png",
    "img/piece/WhiteQueen.png",
    "img/piece/WhiteKing.png",
    "img/piece/BlackPawn.png",
    "img/piece/BlackBishop.png",
    "img/piece/BlackRook.png",
    "img/piece/BlackKnight.png",
    "img/piece/BlackQueen.png",
    "img/piece/BlackKing.png",
};

// basic chess pieces
class Piece{
public:
    Piece();
    ~Piece();
    void setImage(const std::string& path);
    void setVal(int _val);
    void eaten();
    void draw(SDL_Rect& rect);

public:
    int getVal();
    int getSide();
    bool ally(Piece& other);
    bool enemy(Piece& other);
    bool isPiece();

private:
    Texture image;
    int val = -1;
    bool side;
};
