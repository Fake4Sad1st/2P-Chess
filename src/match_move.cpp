#include "match.hpp"

//detail of the move
void Match::move(){
    SDL_Event e;
    while( SDL_PollEvent( &e ) != 0 ) {
        if( e.type == SDL_QUIT ){ Basic::instance().rageQuit(); return;}
        if( e.type == SDL_KEYDOWN ){
            if( e.key.keysym.sym == SDLK_q ){quit = true; return;}
            if( e.key.keysym.sym == SDLK_r ){startNewMatch(); return;} //reset field
            if( e.key.keysym.sym == SDLK_2 ){chessKind ^= 1; return;}
            if( e.key.keysym.sym == SDLK_ESCAPE ) promote = -1, holdPiece = false; //cancel move
        }
        if( e.type == SDL_MOUSEBUTTONDOWN ) {
            int y_board = (e.motion.x < XOFF ? -1 : (e.motion.x - XOFF) / 80);
            int x_board = (e.motion.y < YOFF ? -1 : (e.motion.y - YOFF) / 80);
            if( !insideBoard(x_board, y_board) ) continue;
            if( e.button.button == SDL_BUTTON_LEFT ){
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
            }

            //debug only
            if( e.button.button == SDL_BUTTON_RIGHT ){
                if(board.getValue(x_board, y_board) == -1) continue;
                board.setValue(x_board, y_board, -1);
                reCalculate = true;
            }
        }
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
    dMove.push_back(Movement(from, to, piece[xFrom][yFrom].getVal(),
                             piece[xTo][yTo].getVal(), promoteTo, speCase));
    drawAnimation_SFX(dMove.back());
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
    if(currentSide == WHITE) cerr << "WHITE moves\n";
    else cerr << "BLACK moves\n";
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
