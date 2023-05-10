#include "match.hpp"

//cal the possible move for each side
void Match::calculate(){
    if(notEnoughPiece()){
        outcome(DRAW_AUTOMATIC);
        return;
    }

    if(!dMove.empty()){
        Movement& o = dMove.back();
        if(o.pieceKind == PAWN || o.pieceTaken != -1){
            numDelayMove = 0;
            cb_Saver.clear();
        }
        else if(++numDelayMove == MAX_DELAY_MOVE){outcome(DRAW_AUTOMATIC); return;}
        cerr << "Delay Move = " << numDelayMove << '\n';
    }

    if(repetitionCheck()){
        outcome(DRAW_AUTOMATIC);
        return;
    }

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
    outcome(possibleMove);
}

bool Match::notEnoughPiece(){
    bool addPiece = false;
    FU(i, 0, 8) FU(j, 0, 8){
        int a = piece[i][j].getVal();
        if(a != -1 && a != KING){
            if(addPiece == true || (a != KNIGHT && a != BISHOP))  return false;
            addPiece = true;
        }
    }
    return true;
}

bool Match::repetitionCheck(){
    vector<int> v;
    FU(i, 0, 8) FU(j, 0, 8) v.push_back(board.getValue(i, j));
    if(++cb_Saver[v] == 3) return true;
    return false;
}

//outcome of the match
void Match::outcome(short flag){
    if(flag == POSSIBLE_MOVE) return;
    game_overSFX.play();
    endFlag = true;
    //stop the possible move
    FU(i, 0, 8) FU(j, 0, 8) canMoveTo[i][j] = 0;

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
