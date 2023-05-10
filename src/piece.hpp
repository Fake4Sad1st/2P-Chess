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
const std::string PieceLink[2][12] = {
    {   "img/piece/1/wp.png",
        "img/piece/1/wb.png",
        "img/piece/1/wr.png",
        "img/piece/1/wn.png",
        "img/piece/1/wq.png",
        "img/piece/1/wk.png",
        "img/piece/1/bp.png",
        "img/piece/1/bb.png",
        "img/piece/1/br.png",
        "img/piece/1/bn.png",
        "img/piece/1/bq.png",
        "img/piece/1/bk.png",
    },
    {   "img/piece/2/wp.png",
        "img/piece/2/wb.png",
        "img/piece/2/wr.png",
        "img/piece/2/wn.png",
        "img/piece/2/wq.png",
        "img/piece/2/wk.png",
        "img/piece/2/bp.png",
        "img/piece/2/bb.png",
        "img/piece/2/br.png",
        "img/piece/2/bn.png",
        "img/piece/2/bq.png",
        "img/piece/2/bk.png",
    },
};

// basic chess pieces
class Piece{
public:
    Piece();
    ~Piece();
    void setImage(const std::string& path);
    void setVal(bool chessKind, int _val);
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
