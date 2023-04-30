#pragma once

#include "piece.hpp"
#include "chessboard.hpp"

#define pa pair<int, int>
#define fi first
#define se second

//move in a chess board:
// +) 0 -> 3: bishop
// +) 4 -> 7: rook
// +) 0 -> 7: queen
// +) 8 -> 15: knight
const int XMOVE[] = {1, 1, -1, -1, 0, 1, 0, -1, 1, 1, 2, 2, -1, -1, -2, -2};
const int YMOVE[] = {1, -1, 1, -1, 1, 0, -1, 0, 2, -2, 1, -1, 2, -2, 1, -1};

//store the move in a match
class Movement{
public:
    Movement();
    Movement(pa _from, pa _to, int _pieceKind, bool _pieceTaken);
    pa from, to;

private:
    int pieceKind, pieceTaken;
};


//store the match details
class Match{
public:
    Match();
    ~Match();
    void init();
    void mainEvent();
    void draw();
    void move();
    void calculate();

public:
    void add_numMove();
    void addColorSquare(pa X);

public:
    bool insideBoard(int x, int y){
        return 0<=x && x<8 && 0<=y && y<8;
    }
    int realPos(int x, int y){
        return x * 8 + y;
    }

private:
    Chessboard board;
    SDL_Rect square[8][8];
    Piece piece[8][8];
    Uint64 movable[8][8];
    deque<Movement> dMove;
    int numMove, numTurn;
    bool canMoveTo[8][8];
    bool quit, hold_piece;
    bool currentSide;
    pa cur;
    Texture dot1, dot2;
};
