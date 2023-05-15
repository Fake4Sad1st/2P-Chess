#include "match.hpp"

//detail of the move
void Match::move(){
    SDL_Event e;
    while( SDL_PollEvent( &e ) != 0 ) {
        if( e.type == SDL_QUIT ){ Basic::instance().rageQuit(); return;}
        if( e.type == SDL_KEYDOWN ){
            if( e.key.keysym.sym == SDLK_s ){
                writeMatchReport();
                return;
            }
            if( e.key.keysym.sym == SDLK_ESCAPE ) promote = -1, holdPiece = false; //cancel move
        }
        if( e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT ) {
            int x = e.motion.x, y = e.motion.y;
            bool pass = state == NORMAL_SCR;
            checkButton(x, y);
            if(!pass) return;
            int y_board = (x < XOFF ? -1 : (x - XOFF) / 80);
            int x_board = (y < YOFF ? -1 : (y - YOFF) / 80);
            if( !insideBoard(x_board, y_board) ) continue;
            if (holdPiece == true) {
                if (promote != -1){
                    if(tempBoard[x_board][y_board] != -1){
                        holdPiece = false;
                        saveMove(cur, make_pair((currentSide == WHITE ? 0: 7), promote), tempBoard[x_board][y_board]);
                        promote = -1;
                        add_numMove();
                    }
                    else promote = -1, holdPiece = false;
                    continue;
                }
                holdPiece = false;
                if(!canMoveTo[x_board][y_board]){
                    if(cur != make_pair(x_board, y_board) && piece[x_board][y_board].isPiece())
                        cur = make_pair(x_board, y_board), holdPiece = true;
                    continue;
                }
                if(piece[cur.fi][cur.se].getVal() == PAWN && (x_board == 0 || x_board == 7)){
                    holdPiece = true;
                    create_promotionBoard(y_board);
                    continue;
                }

                saveMove(cur, make_pair(x_board, y_board));
                add_numMove();
            }
            else {
                if (piece[x_board][y_board].isPiece()) {
                    holdPiece = true;
                    cur = make_pair(x_board, y_board);
                }
            }

            ///debug only
//            if( e.button.button == SDL_BUTTON_RIGHT ){
//                if(board.getValue(x_board, y_board) == -1) continue;
//                board.setValue(x_board, y_board, -1);
//                reCalculate = true;
//            }
        }
    }
}

void Match::checkButton(int x, int y){
    switch (state){
        case NORMAL_SCR:{
            if(Mix_VolumeMusic(-1) != 0){
                if(audio.click(x, y)) Mix_VolumeMusic(0);
            }
            else if(no_audio.click(x, y)) Mix_VolumeMusic(DEFAULT_AUDIO_VOL);

            if(Mix_MasterVolume(-1) != 0){
                if(sfx.click(x, y)) Mix_MasterVolume(0);
            }
            else if(no_sfx.click(x, y)) Mix_MasterVolume(DEFAULT_SFX_VOL);
            if(setting.click(x, y)) state = SETTING_SCR;
            if(change_pieces.click(x, y)) chessKind ^= 1;

            if(!endFlag){
                if(numTurn >= 3){
                    if(draw_button.click(x, y)) state = DRAW_SCR;
                    if(resign_button.click(x, y)) state = RESIGN_SCR;
                }
            }
            else if(!saveMatch && matchSave_button.click(x, y)) SDL_Delay(250), writeMatchReport();

        }
        break;
        case SETTING_SCR:{
            int o = settingBox.click(x, y);
            if(o == 0) state = NORMAL_SCR;
            if(o == 1){
                if(endFlag) startNewMatch(); //reset field
                else state = REPLAY_SCR;
            }
            if(o == 2){
                if(endFlag) quit = true;
                else state = QUIT_SCR;
            }
        }
        break;
        case REPLAY_SCR:{
            int o = replay_prompt.click(x, y);
            if(o == 0) startNewMatch(), state = NORMAL_SCR;
            if(o == 1) state = SETTING_SCR;
        }
        break;
        case QUIT_SCR:{
            int o = quit_prompt.click(x, y);
            if(o == 0) quit = true;
            if(o != -1) state = SETTING_SCR;
        }
        break;
        case DRAW_SCR:{
            int o = draw_prompt.click(x, y);
            if(o == 0) outcome(DRAW_OFFER);
            if(o != -1) state = NORMAL_SCR;
        }
        break;
        case RESIGN_SCR:{
            int o = (currentSide == WHITE ? resignWhite_prompt.click(x, y) :
                                            resignBlack_prompt.click(x, y));
            if(o == 0) outcome(RESIGN);
            if(o != -1) state = NORMAL_SCR;
        }
        break;
    }
}

//save the move you made to the board
void Match::saveMove(pa from, pa to, int promoteTo){
    int xFrom = from.fi, yFrom = from.se;
    int xTo = to.fi, yTo = to.se;
    hasMoved[xFrom][yFrom] = hasMoved[xTo][yTo] = true;

    //Pawn promotion;
    if(promoteTo != -1) board.setValue(xTo, yTo, promoteTo);
    else board.setValue(xTo, yTo, board.getValue(xFrom, yFrom));
    board.setValue(xFrom, yFrom, -1);

    short speCase = NOTHING;
    if(piece[xFrom][yFrom].getVal() == PAWN){
         if( abs(yTo - yFrom) == 1 && !piece[xTo][yTo].isPiece()){ //En passant detected
            speCase = EN_PASSANT;
            board.setValue(xFrom, yTo, -1);
         }
         else if( xTo == 0 || xTo == 7 ) speCase = PROMOTING;
    }
    else if(piece[xFrom][yFrom].getVal() == KING && abs(yTo - yFrom) == 2){ //Castling detected
        speCase = CASTLING;
        if(yTo == 2){//far castling: o-o-o
            board.setValue(xTo, 3, NUM_OF_PIECES * currentSide + ROOK);
            board.setValue(xTo, 0, -1);
        }
        else{//close castling: o-o
            board.setValue(xTo, 5, NUM_OF_PIECES * currentSide + ROOK);
            board.setValue(xTo, 7, -1);
        }
    }
    Movement o(from, to, piece[xFrom][yFrom].getVal(),
                piece[xTo][yTo].getVal(), promoteTo, speCase);
    dMove.push_back(o);
    saveMoveSign(o);
    drawAnimation_SFX(o);
}

//go to a new move
void Match::add_numMove(){
    currentSide ^= 1;
    reCalculate = true; draw();
    if(endFlag) return;

    ++numMove;
    if(numMove & 1){
        ++numTurn;
        cerr << "Turns no. " << numTurn << endl;
    }
}


//check if the board is checked
bool Match::isThatCheck(bool sideGotChecked, pa From, pa To, short speCase){
    create_tempBoard(From.fi, From.se, To.fi, To.se, speCase);
    FU(i, 0, 8) FU(j, 0, 8){
        int o = tempBoard[i][j];
        if(o == -1) continue;
        if(o / 6 == sideGotChecked){
            if(o % 6 == KING) tempBoard[i][j] = -2;
            else tempBoard[i][j] = -3;
        }
        else tempBoard[i][j] %= 6;
    }
    return check_tempBoard(sideGotChecked ^ 1);
}

void Match::create_tempBoard(int xFrom, int yFrom, int xTo, int yTo, short speCase){
    FU(i, 0, 8) FU(j, 0, 8) tempBoard[i][j] = board.getValue(i, j);
    if(xFrom != -1){
        tempBoard[xTo][yTo] = tempBoard[xFrom][yFrom];
        tempBoard[xFrom][yFrom] = -1;
        if(speCase == CASTLING){
            assert(yFrom == 4 && xFrom == xTo);
            if(yTo == 2){//far castling: o-o-o
                tempBoard[xTo][3] = tempBoard[xTo][0];
                tempBoard[xTo][0] = -1;
            }
            else{// close castling: o-o
                tempBoard[xTo][5] = tempBoard[xTo][7];
                tempBoard[xTo][7] = -1;
            }
        }
        else if(speCase == EN_PASSANT){
            assert(abs(yFrom - yTo) == 1 && abs(xFrom - xTo) == 1);
            tempBoard[xFrom][yTo] = -1;
        }
    }
}

bool Match::check_tempBoard(bool Side){
    FU(i, 0, 8) FU(j, 0, 8) if(tempBoard[i][j] >= 0){
        switch (tempBoard[i][j]){
            case PAWN:
                {
                    int dir = (Side == WHITE ? -1: 1);
                    if(insideBoard(i + dir, j - 1) && tempBoard[i + dir][j - 1] == -2) return true;
                    if(insideBoard(i + dir, j + 1) && tempBoard[i + dir][j + 1] == -2) return true;
                }
                break;
            case KNIGHT:
                FU(k, 8, 16){
                    int x = i + XMOVE[k], y = j + YMOVE[k];
                    if(insideBoard(x, y) && tempBoard[x][y] == -2) return true;
                }
                break;
            case BISHOP:
                FU(k, 0, 4){
                    int x = i, y = j;
                    FU(step, 1, 8){
                        x += XMOVE[k], y += YMOVE[k];
                        if(!insideBoard(x, y) || tempBoard[x][y] == -3 || tempBoard[x][y] >= 0) break;
                        if(tempBoard[x][y] == -2) return true;
                    }
                }
                break;
            case ROOK:
                FU(k, 4, 8){
                    int x = i, y = j;
                    FU(step, 1, 8){
                        x += XMOVE[k], y += YMOVE[k];
                        if(!insideBoard(x, y) || tempBoard[x][y] == -3 || tempBoard[x][y] >= 0) break;
                        if(tempBoard[x][y] == -2) return true;
                    }
                }
                break;
            case QUEEN:
                FU(k, 0, 8){
                    int x = i, y = j;
                    FU(step, 1, 8){
                        x += XMOVE[k], y += YMOVE[k];
                        if(!insideBoard(x, y) || tempBoard[x][y] == -3 || tempBoard[x][y] >= 0) break;
                        if(tempBoard[x][y] == -2) return true;
                    }
                }
                break;
            case KING:
                FU(k, 0, 8){
                    int x = i + XMOVE[k], y = j + YMOVE[k];
                    if(insideBoard(x, y) && tempBoard[x][y] == -2) return true;
                }
                break;
            default:{cerr << "Error: No piece detected\n"; throw;}
        }
    }
    return false;
}

const string PieceSign = " BRNQK";
void Match::saveMoveSign(const Movement& X){
    string Sign;
    string pF = signPos(X.from), pT = signPos(X.to);
    int pK = X.pieceKind;

    if(pK == PAWN){
        if(X.pieceTaken != -1 || X.speCase == EN_PASSANT) Sign += pF[0], Sign += 'x';
        Sign += pT;
        if(X.speCase == PROMOTING) Sign += '=' , Sign += PieceSign[X.promoteTo];
    }
    else if(X.speCase == CASTLING){
        if(X.to.se == 2) Sign = "O-O-O";
        else Sign = "O-O";
    }
    else{
        Sign = PieceSign[X.pieceKind];
        //check if another piece of the same kind could move to that square
        bool existed = 0, sameCol = 0, sameRow = 0;
        FU(i, 0, 8) FU(j, 0, 8){
            if(make_pair(i, j) == X.from) continue;
            if(piece[i][j].getSide() == currentSide && piece[i][j].getVal() == X.pieceKind
                && ONBIT(movable[i][j], realPos(X.to.fi, X.to.se))){
                existed = 1;
                if(i == X.from.fi) sameRow = 1;
                if(j == X.from.se) sameCol = 1;
            }
        }
        if(existed){
            if(sameCol){
                if(sameRow) Sign += pF;
                else Sign += pF[1];
            }
            else Sign += pF[0];
        }
        if(X.pieceTaken != -1) Sign += 'x';
        Sign += pT;
    }

    ///Calculate check here
    inCheck = isThatCheck(currentSide ^ 1);
    if(inCheck) Sign += '+';
    signMove.push_back(Sign);

//    cerr << Sign <<'\n';
}
