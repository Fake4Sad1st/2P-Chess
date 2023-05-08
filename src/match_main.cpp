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

    quit = false;
    FU(i, 0, 8) FU(j, 0, 8){
        square[i][j] = SDL_Rect{XOFF + j * PIECE_SIZE , YOFF + i * PIECE_SIZE, PIECE_SIZE, PIECE_SIZE};
        smSquare[i][j] = SDL_Rect{XOFF - 20 + (j+1) * PIECE_SIZE , max(0, YOFF - 12 + i * PIECE_SIZE), 32, 32};
    }
}

Match::~Match(){}

//distinguish music from the chess match
void Match::init(){
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
    numMove = numTurn = 0;
    promote = -1;
    cur = make_pair(-1, -1);
    board.initValue();
    FU(i, 0, 8) FU(j, 0, 8) hasMoved[i][j] = false;
    dMove.clear();

    add_numMove();
}

void Match::mainEvent(){
    init();
    while(!quit){
        draw();
        move();
        if(Basic::instance().askQuit()) return;
    }
    finish();
}

//cal the possible move for each side
void Match::calculate(){
    possibleMove = 0;

    FU(i, 0, 8) FU(j, 0, 8){
        movable[i][j] = 0;
        if(piece[i][j].getSide() != currentSide) continue;
        switch(piece[i][j].getVal()){
            case PAWN:
                {
                    int dir, startRow;
                    if(currentSide == WHITE) dir = -1, startRow = 6;
                    else dir = 1, startRow = 1;

                    if(insideBoard(i + dir, j) && !piece[i + dir][j].isPiece()){
                        if(!isThatCheck(currentSide, make_pair(i, j), make_pair(i+dir, j)))
                            addBit(movable[i][j], realPos(i + dir, j));
                        if(i == startRow && !piece[i + 2*dir][j].isPiece()
                           && !isThatCheck(currentSide, make_pair(i, j), make_pair(i+2*dir, j)))
                            addBit(movable[i][j], realPos(i + 2*dir, j));
                    }
                    if(insideBoard(i + dir, j - 1) && piece[i][j].enemy(piece[i + dir][j - 1])
                       && !isThatCheck(currentSide, make_pair(i, j), make_pair(i+dir, j-1)))
                        addBit(movable[i][j], realPos(i + dir, j - 1));
                    if(insideBoard(i + dir, j + 1) && piece[i][j].enemy(piece[i + dir][j + 1])
                       && !isThatCheck(currentSide, make_pair(i, j), make_pair(i+dir, j+1)))
                        addBit(movable[i][j], realPos(i + dir, j + 1));

                    //en passant
                    if(i == startRow + 3*dir){
                        if(dMove.empty() || dMove.back().pieceKind != PAWN) break;
                        pa From = dMove.back().from, To = dMove.back().to;
                        if(abs(From.fi - To.fi) != 2 || abs(j - To.se) != 1) break;
                        if(!isThatCheck(currentSide, make_pair(i, j), make_pair(i+dir, To.se), EN_PASSANT))
                            addBit(movable[i][j], realPos(i + dir, To.se));
                    }
                }
                break;
            case KNIGHT:
                FU(k, 8, 16){
                    int x = i + XMOVE[k], y = j + YMOVE[k];
                    if(insideBoard(x, y) && !piece[i][j].ally(piece[x][y])
                       && !isThatCheck(currentSide, make_pair(i, j), make_pair(x, y)))
                        addBit(movable[i][j], realPos(x, y));
                }
                break;
            case BISHOP:
                FU(k, 0, 4){
                    int x = i, y = j;
                    FU(step, 1, 8){
                        x += XMOVE[k], y += YMOVE[k];
                        if(!insideBoard(x, y) || piece[i][j].ally(piece[x][y])) break;
                        if(!isThatCheck(currentSide, make_pair(i, j), make_pair(x, y)))
                            addBit(movable[i][j], realPos(x, y));
                        if(piece[x][y].isPiece()) break;
                    }
                }
                break;
            case ROOK:
                FU(k, 4, 8){
                    int x = i, y = j;
                    FU(step, 1, 8){
                        x += XMOVE[k], y += YMOVE[k];
                        if(!insideBoard(x, y) || piece[i][j].ally(piece[x][y])) break;
                        if(!isThatCheck(currentSide, make_pair(i, j), make_pair(x, y)))
                            addBit(movable[i][j], realPos(x, y));
                        if(piece[x][y].isPiece()) break;
                    }
                }
                break;
            case QUEEN:
                FU(k, 0, 8){
                    int x = i, y = j;
                    FU(step, 1, 8){
                        x += XMOVE[k], y += YMOVE[k];
                        if(!insideBoard(x, y) || piece[i][j].ally(piece[x][y])) break;
                        if(!isThatCheck(currentSide, make_pair(i, j), make_pair(x, y)))
                            addBit(movable[i][j], realPos(x, y));
                        if(piece[x][y].isPiece()) break;
                    }
                }
                break;
            case KING:
                {
                    bool ok[8];
                    FU(k, 0, 8){
                        int x = i + XMOVE[k], y = j + YMOVE[k];
                        ok[k] = 1;
                        if(!insideBoard(x, y) || piece[i][j].ally(piece[x][y])) continue;
                        ok[k] = isThatCheck(currentSide, make_pair(i, j), make_pair(x, y));
                        if(!ok[k]) addBit(movable[i][j], realPos(x, y));
                    }

                    //castling
                    if(!inCheck && hasMoved[i][j] == false){
                        assert(j == 4);
                        if(!ok[4]){//not be attacked when moving close
                            if(hasMoved[i][7] == false && !piece[i][5].isPiece() && !piece[i][6].isPiece()
                               && !isThatCheck(currentSide, make_pair(i, 4), make_pair(i, 6), CASTLING))
                                addBit(movable[i][4], realPos(i, 6));
                        }
                        if(!ok[6]){//not be attacked when moving far
                            if(hasMoved[i][0] == false && !piece[i][1].isPiece() && !piece[i][2].isPiece() && !piece[i][3].isPiece()
                               && !isThatCheck(currentSide, make_pair(i, 4), make_pair(i, 2), CASTLING))
                                addBit(movable[i][4], realPos(i, 2));
                        }
                    }
                }
                break;
            default:{cerr << "Error: No piece detected\n"; throw;}
        }
    }

    if(!endFlag) outcome(possibleMove);
}

//outcome of the match
void Match::outcome(short flag){
    if(flag == POSSIBLE_MOVE) return;
    game_overSFX.play();
    endFlag = true;
    switch(flag){
        case NO_POSSIBLE_MOVE:
            if(inCheck){
                if(currentSide == WHITE){
                    cerr << "Black";
                    WKingSym = Texture(CheckmateWhiteLink);
                    BKingSym = Texture(WinnerLink);
                }
                else{
                    cerr << "White";
                    WKingSym = Texture(WinnerLink);
                    BKingSym = Texture(CheckmateBlackLink);
                }
                cerr << " wins by checkmate!\n";
            }
            else{
                WKingSym = Texture(DrawWhiteLink);
                BKingSym = Texture(DrawBlackLink);
                cerr << "Draw by stalemate!\n";
            }
            break;
        case RESIGN:
            if(currentSide == WHITE){
                cerr << "White";
                WKingSym = Texture(WinnerLink);
                BKingSym = Texture(ResignBlackLink);
            }
            else{
                cerr << "Black";
                WKingSym = Texture(ResignWhiteLink);
                BKingSym = Texture(WinnerLink);
            }
            cerr << " wins.\n Opponent resigns!\n";
            break;
        case DRAW_OFFER:
            WKingSym = Texture(DrawWhiteLink);
            BKingSym = Texture(DrawBlackLink);
            cerr << "Draw by offer!\n";
            break;
        case DRAW_AUTOMATIC:
            WKingSym = Texture(DrawWhiteLink);
            BKingSym = Texture(DrawBlackLink);
            cerr << "Automatic draw!\n";
            break;
    }
}
