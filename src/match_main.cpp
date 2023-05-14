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
Match::Match():
    audio(SDL_Rect {950, 10, 50, 50}, "img/audio.png"),
    no_audio(SDL_Rect {950, 10, 50, 50}, "img/no_audio.png"),
    sfx(SDL_Rect {885, 10, 50, 50}, "img/sfx.png"),
    no_sfx(SDL_Rect {885, 10, 50, 50}, "img/no_sfx.png"),
    setting(SDL_Rect {820, 10, 50, 50}, "img/settings_icon.png"),
    change_pieces(SDL_Rect {683, 80, 320, 80}, "img/change_pieces.png"),
    draw_button(SDL_Rect {700, 590, 135, 50}, "img/draw_button.png"),
    resign_button(SDL_Rect {855, 590, 135, 50}, "img/resign_button.png"),
    matchSave_button(SAVE_MATCH_RECT, "img/matchSave_button.png"),

    settingBox(SDL_Rect {256, 110, 500, 495}, "img/settings_box2.png"),

    matchSaved_button("img/matchSaved_button.png")
{
    //box
    settingBox.setRect(vector<SDL_Rect> {
                        SDL_Rect {60, 90, 379, 110},
                        SDL_Rect {60, 215, 379, 110},
                        SDL_Rect {60, 340, 379, 110} });
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
    cerr << "START NEW MATCH...\n";

    state = NORMAL_SCR;
    reCalculate = holdPiece = false;
    inCheck = endFlag = saveMatch = false;
    currentSide = BLACK;
    numMove = numTurn = numDelayMove = 0;
    promote = -1;
    cur = make_pair(-1, -1);
    board.initValue();
    FU(i, 0, 8) FU(j, 0, 8) hasMoved[i][j] = false;
    dMove.clear(); cb_Saver.clear(); signMove.clear();

    add_numMove();
}
