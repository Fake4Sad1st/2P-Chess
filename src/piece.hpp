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
    KNIGHT,
    ROOK,
    QUEEN,
    KING,
    NUM_OF_PIECES,
};

//image links
const std::string PieceLink[] = {
    "img/piece/WhitePawn.png",
    "img/piece/WhiteBishop.png",
    "img/piece/WhiteKnight.png",
    "img/piece/WhiteRook.png",
    "img/piece/WhiteQueen.png",
    "img/piece/WhiteKing.png",
    "img/piece/BlackPawn.png",
    "img/piece/BlackBishop.png",
    "img/piece/BlackKnight.png",
    "img/piece/BlackRook.png",
    "img/piece/BlackQueen.png",
    "img/piece/BlackKing.png",
};

// basic chess pieces
class Piece{
public:
    Piece();
    ~Piece();
    void setImage(const std::string& path);
    void setPos(int _x,int _y);
    void setVal(int _val);
    void eaten();
    void draw(SDL_Rect& rect);

public:
    int getx();
    int gety();
    int getVal();
    int getSide();
    bool ally(Piece& other);
    bool enemy(Piece& other);
    bool isPiece();

private:
    Texture image;
    int x, y;
    int val = -1;
    bool side;
};

enum ChessKind{
    WHITE_PAWN,
    WHITE_BISHOP,
    WHITE_KNIGHT,
    WHITE_ROOK,
    WHITE_QUEEN,
    WHITE_KING,
    BLACK_PAWN,
    BLACK_BISHOP,
    BLACK_KNIGHT,
    BLACK_ROOK,
    BLACK_QUEEN,
    BLACK_KING,
};
