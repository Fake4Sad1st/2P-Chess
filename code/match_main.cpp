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
    audio(SDL_Rect {950, 10, 50, 50}, "source/img/icon/audio.png"),
    no_audio(SDL_Rect {950, 10, 50, 50}, "source/img/icon/no_audio.png"),
    sfx(SDL_Rect {885, 10, 50, 50}, "source/img/icon/sfx.png"),
    no_sfx(SDL_Rect {885, 10, 50, 50}, "source/img/icon/no_sfx.png"),
    setting(SDL_Rect {820, 10, 50, 50}, "source/img/icon/settings_icon.png"),
    change_pieces(SDL_Rect {683, 80, 320, 80}, "source/img/icon/change_pieces.png"),
    draw_button(SDL_Rect {700, 560, 135, 50}, "source/img/icon/draw_button.png"),
    resign_button(SDL_Rect {855, 560, 135, 50}, "source/img/icon/resign_button.png"),
    matchSave_button(SAVE_MATCH_RECT, "source/img/icon/matchSave_button.png"),

    settingBox(SDL_Rect {256, 110, 500, 495}, "source/img/icon/settings_box2.png"),
    replay_prompt(PROMPT_RECT, "source/img/icon/replay_prompt.png"),
    quit_prompt(PROMPT_RECT, "source/img/icon/quit_prompt.png"),
    draw_prompt(PROMPT_RECT, "source/img/icon/draw_prompt.png"),
    resignBlack_prompt(PROMPT_RECT, "source/img/icon/resignBlack_prompt.png"),
    resignWhite_prompt(PROMPT_RECT, "source/img/icon/resignWhite_prompt.png"),

    matchSaved_button("source/img/icon/matchSaved_button.png"),
    circleOn("source/img/icon/circleOn.png")
{
    //box
    vector<SDL_Rect> V = { SDL_Rect {60, 90, 379, 110},
                           SDL_Rect {60, 215, 379, 110},
                           SDL_Rect {60, 340, 379, 110} };
    settingBox.setRect(V);

    V = { SDL_Rect {30, 170, 150, 50},
          SDL_Rect {220, 170, 150, 50} };
    replay_prompt.setRect(V);
    quit_prompt.setRect(V);
    draw_prompt.setRect(V);
    resignWhite_prompt.setRect(V);
    resignBlack_prompt.setRect(V);
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
    //font
    font = TTF_OpenFont("source/font/DigitalSans.ttf", 50);

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
