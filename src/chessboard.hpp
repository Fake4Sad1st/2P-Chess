#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <vector>

#include "texture.hpp"
#include "game.hpp"

using namespace std;

//Chessboard off the border size
const int XOFF = 24;
const int YOFF = 9;

//Image link
const std::string chessboardLink = "img/chessboard.png";

class Chessboard{
public:
    Chessboard();
    ~Chessboard();
    void initValue();
    void draw();
    int getValue(int x, int y);
    void setValue(int x, int y, int _val);

private:
    Texture chessboard;
    vector< vector<int> > value;

};


enum ChessKind{
    WPAWN,
    WBISHOP,
    WROOK,
    WKNIGHT,
    WQUEEN,
    WKING,
    BPAWN,
    BBISHOP,
    BROOK,
    BKNIGHT,
    BQUEEN,
    BKING,
};
