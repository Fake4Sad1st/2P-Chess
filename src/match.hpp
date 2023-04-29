#pragma once

#include "piece.hpp"
#include "chessboard.hpp"

#define pa pair<int, int>
#define fi first
#define se second

//store the move in a match
class Movement{
public:
    Movement();
    Movement(pa _from, pa _to, int _pieceKind, bool _takePiece);
    pa from, to;

private:
    int pieceKind;
    bool takePiece;
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

public:
    void add_numMove();
    void addColorSquare(pa X);

private:
    Chessboard board;
    SDL_Rect square[8][8];
    Piece piece[8][8];
    deque<Movement> dMove;
    int numMove, numTurn;
    bool quit = false, hold_piece = false;
    bool currentSide;
    pa cur;
};
