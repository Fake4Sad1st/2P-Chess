#include "match.hpp"

///about Movement class
Movement::Movement(){}

Movement::Movement(pa _from, pa _to, int _pieceKind, int _pieceTaken,
                   int _promoteTo, short _speCase):
    from(_from), to(_to), pieceKind(_pieceKind), pieceTaken(_pieceTaken),
    promoteTo(_promoteTo), speCase(_speCase) {}


///about Change class
Change::Change(){}

Change::Change(pa _from, pa _to, int _pieceIn):
    Case(MOVED), from(_from), to(_to), pieceIn(_pieceIn) {}

Change::Change(pa pos, int _pieceIn):
    Case(EATEN), from(pos), to(make_pair(-1, -1)), pieceIn(_pieceIn) {}


///about Match class
Match::Match(){
    //texture
    dot1 = Texture(Dot1Link); dot1.setAlpha(0x78);
    dot2 = Texture(Dot2Link); dot2.setAlpha(0x78);
    //music
    theme = Music(Theme2Link);
    //sfx
    captureSFX = SFX(CaptureSFXLink);
    castleSFX = SFX(CastleSFXLink);
    checkSFX = SFX(CheckSFXLink);
    game_overSFX = SFX(GameOverSFXLink);
    moveSFX = SFX(MoveSFXLink);
    promoteSFX = SFX(PromoteSFXLink);

    chessKind = 0;
    FU(i, 0, 8) FU(j, 0, 8){
        square[i][j] = SDL_Rect{XOFF + j * PIECE_SIZE , YOFF + i * PIECE_SIZE, PIECE_SIZE, PIECE_SIZE};
        smSquare[i][j] = SDL_Rect{XOFF - 20 + (j+1) * PIECE_SIZE , max(0, YOFF - 12 + i * PIECE_SIZE), 32, 32};
    }
}

Match::~Match(){}

void Match::mainEvent(){
    init();
    while(!quit){
        draw();
        move();
        if(Basic::instance().askQuit()) return;
    }
    finish();
}

//distinguish music from the chess match
void Match::init(){
    quit = false;
    theme.play();
    startNewMatch();
}

void Match::finish(){
    theme.stop();
}

void Match::startNewMatch(){
    reCalculate = holdPiece = false;
    inCheck = endFlag = false;
    currentSide = BLACK;
    numMove = numTurn = numDelayMove = 0;
    promote = -1;
    cur = make_pair(-1, -1);
    board.initValue();
    FU(i, 0, 8) FU(j, 0, 8) hasMoved[i][j] = false;
    dMove.clear(); cb_Saver.clear();

    add_numMove();
}
