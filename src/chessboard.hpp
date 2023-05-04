#pragma once

#include <vector>

#include "texture.hpp"

using namespace std;

//Chessboard off the border size
const int XOFF = 24;
const int YOFF = 9;

//Image link
const std::string ChessboardLink = "img/background/chessboard.png";

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
