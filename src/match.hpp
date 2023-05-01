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

//the cases happen when moving a piece
enum MOVING_CASE{
    NOTHING,
    CASTLING,
    EN_PASSANT,
};

//store the move in a match
class Movement{
public:
    Movement();
    Movement(pa _from, pa _to, int _pieceKind, bool _pieceTaken);
    pa from, to;
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
    void saveMove(pa from, pa to);
    void add_numMove();
    void addColorSquare(pa X);
    void addBit(Uint64 &x, int b);

//for check
public:
    bool isThatCheck(bool sideGotChecked, pa From = make_pair(-1, -1), pa To = make_pair(-1, -1), short speCase=NOTHING);
    void create_tempBoard(int xFrom, int yFrom, int xTo, int yTo, short speCase);
    bool check_tempBoard(bool Side);
private:
    bool inCheck, suitableMove;

public:
    bool insideBoard(int x, int y){
        return 0<=x && x<8 && 0<=y && y<8;
    }
    int realPos(int x, int y){
        return x * 8 + y;
    }

private:
    Chessboard board;
    Piece piece[8][8];
    SDL_Rect square[8][8];
    Uint64 movable[8][8];
    deque<Movement> dMove;
    int numMove, numTurn;
    int tempBoard[8][8];
    bool canMoveTo[8][8], reCalculate;
    bool hasMoved[8][8];
    bool quit, hold_piece;
    bool currentSide;
    pa cur;
    Texture dot1, dot2;
};
