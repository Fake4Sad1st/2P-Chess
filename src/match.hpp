#pragma once

#include "piece.hpp"
#include "chessboard.hpp"

#define pa pair<int, int>
#define fi first
#define se second

//dot appeared when moving pieces
const std::string Dot1Link = "img/dots/dot1.png";
const std::string Dot2Link = "img/dots/dot2.png";

//song to play
const std::string Theme2Link = "music/song/sweden.mp3";

//SFXs
const std::string CaptureSFXLink = "music/sfx/capture.wav";
const std::string CastleSFXLink = "music/sfx/castle.wav";
const std::string CheckSFXLink = "music/sfx/check.wav";
const std::string GameOverSFXLink = "music/sfx/game_over.wav";
const std::string MoveSFXLink = "music/sfx/move.wav";
const std::string PromoteSFXLink = "music/sfx/promote.wav";

//move in a chess board:
// +) 0 -> 3: bishop
// +) 4 -> 7: rook
// +) 0 -> 7: queen
// +) 8 -> 15: knight
const int XMOVE[] = {1, 1, -1, -1, 0, 1, 0, -1, 1, 1, 2, 2, -1, -1, -2, -2};
const int YMOVE[] = {1, -1, 1, -1, 1, 0, -1, 0, 2, -2, 1, -1, 2, -2, 1, -1};

//color
const Uint32 COLOR_MOVED = 0xF6F6699F;
const Uint32 COLOR_PROMOTE = 0xFFFFFFFF;

//the cases happen when moving a piece
enum MOVING_CASE{
    NOTHING,
    CASTLING,
    EN_PASSANT,
    PROMOTING,
};

//store the move in a match
class Movement{
public:
    Movement();
    Movement(pa _from, pa _to, int _pieceKind, int _pieceTaken, int _promoteTo, short _speCase = 0);
    pa from, to;
    int pieceKind, pieceTaken, promoteTo;
    short speCase;
};

//about class Change
const int NUM_FRAME = 16;
enum ANIMATION_CASE{
    MOVED,
    EATEN,
};

//store the change of pieces in a step
class Change{
public:
    Change();
    Change(pa _from, pa _to, int _pieceIn); //MOVED
    Change(pa pos, int _pieceIn); //EATEN

public:
    short Case;
    pa from, to;
    int pieceIn;
};

//store the match details
class Match{
public:
    Match();
    ~Match();
    void init();
    void finish();
    void startNewMatch();
    void mainEvent();
    void draw();
    void move();
    void calculate();
    void outcome();

public:
    void drawPromotion();
    void saveMove(pa from, pa to, int promoteTo = -1);
    void add_numMove();
    void addColorSquare(Uint32 val, pa X);
    void addBit(Uint64 &x, int b);

//for check
public:
    bool isThatCheck(bool sideGotChecked, pa From = make_pair(-1, -1), pa To = make_pair(-1, -1), short speCase=NOTHING);
    void create_promotionBoard(int col);
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
    Texture dot1, dot2;
    Music theme;
    SFX captureSFX, castleSFX, checkSFX, game_overSFX, moveSFX, promoteSFX;
    SDL_Rect square[8][8];
    Chessboard board;
    Piece piece[8][8];
    Uint64 movable[8][8];
    vector<Movement> dMove;
    int numMove, numTurn;
    int promote;
    int tempBoard[8][8];
    bool canMoveTo[8][8], reCalculate;
    bool hasMoved[8][8];
    bool quit, hold_piece;
    bool currentSide;
    pa cur;

//For the animation
public:
    void drawAnimation_SFX(const Movement& X);
    void drawAnimationStep(int step, const vector<Change>& ani);
};
