#include "match.hpp"

#define ONBIT(x, i) ((x) >> (i) & 1)
#define UPBIT(x, i) ((x) |= (1ULL << (i)))

///about Movement class
Movement::Movement(){}

Movement::Movement(pa _from, pa _to, int _pieceKind, bool _pieceTaken):
    from(_from), to(_to), pieceKind(_pieceKind), pieceTaken(_pieceTaken) {}


///about Match class
Match::Match(){
    dot1 = Texture("img/dot1.png"); dot1.setAlpha(0x78);
    dot2 = Texture("img/dot2.png"); dot2.setAlpha(0x78);
    FU(i, 0, 8) FU(j, 0, 8)
        square[i][j] = SDL_Rect{XOFF + j * PIECE_SIZE , YOFF + i * PIECE_SIZE,
                                PIECE_SIZE, PIECE_SIZE};
    init();
}

Match::~Match(){}

void Match::init(){
    reCalculate = false;
    quit = hold_piece = false;
    numMove = numTurn = 0;
    currentSide = BLACK;
    cur = make_pair(0, 0);
    board.initValue();
    FU(i, 0, 8) FU(j, 0, 8){
        piece[i][j].setPos(i, j);
        movable[i][j] = 0;
    }
    dMove.clear();

    add_numMove();
}

void Match::mainEvent(){
    while(!quit){
        draw();
        move();
        if(quit) break;
    }
}

//detail of the move
void Match::move(){
    SDL_Event e;
    while( SDL_PollEvent( &e ) != 0 ) {
        if( e.type == SDL_QUIT ){ quit = true; return;}
        if( e.type == SDL_MOUSEBUTTONDOWN ) {
            int y_board = (e.motion.x < XOFF ? -1 : (e.motion.x - XOFF) / 80);
            int x_board = (e.motion.y < YOFF ? -1 : (e.motion.y - YOFF) / 80);
            if( !insideBoard(x_board, y_board) ) continue;
            cerr << x_board << ' ' << y_board << endl;
            if( e.button.button == SDL_BUTTON_LEFT ){
                if (hold_piece == true) {
                    hold_piece = false;
                    if(!canMoveTo[x_board][y_board]){
                        cerr << "CANCELLED!\n";
                        continue;
                    }

                    dMove.push_back(Movement(cur, make_pair(x_board, y_board),
                                            piece[cur.fi][cur.se].getVal(), piece[x_board][y_board].getVal()));
                    board.setValue(x_board, y_board, board.getValue(cur.fi, cur.se));
                    board.setValue(cur.fi, cur.se, -1);

                    add_numMove();
                }
                else {
                    if (piece[x_board][y_board].getSide() == currentSide) {
                        hold_piece = true;
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


///need fixing
//cal the possible move for each side
void Match::calculate(){
    suitableMove = 0;

    inCheck = isThatCheck(currentSide);
    cout << "Check: " << inCheck << endl;

    FU(i, 0, 8) FU(j, 0, 8){
        movable[i][j] = 0;
        if(piece[i][j].getSide() != currentSide) continue;
        switch(piece[i][j].getVal()){
            case PAWN:
                ///Todo: en passant, promotion;
                {
                    int dir, startRow;
                    if(currentSide == WHITE) dir = -1, startRow = 6;
                    else dir = 1, startRow = 1;

                    if(insideBoard(i + dir, j) && !piece[i + dir][j].isPiece()){
                        if(!isThatCheck(currentSide, make_pair(i, j), make_pair(i+dir, j)))
                            addBit(movable[i][j], realPos(i + dir, j));
                        if(i == startRow && insideBoard(i + 2*dir, j) && !piece[i + 2*dir][j].isPiece()
                           && !isThatCheck(currentSide, make_pair(i, j), make_pair(i+2*dir, j)))
                            addBit(movable[i][j], realPos(i + 2*dir, j));
                    }
                    if(insideBoard(i + dir, j - 1) && piece[i][j].enemy(piece[i + dir][j - 1])
                       && !isThatCheck(currentSide, make_pair(i, j), make_pair(i+dir, j-1)))
                        addBit(movable[i][j], realPos(i + dir, j - 1));
                    if(insideBoard(i + dir, j + 1) && piece[i][j].enemy(piece[i + dir][j + 1])
                       && !isThatCheck(currentSide, make_pair(i, j), make_pair(i+dir, j+1)))
                        addBit(movable[i][j], realPos(i + dir, j + 1));
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
                ///Todo: castling
                FU(k, 0, 8){
                    int x = i + XMOVE[k], y = j + YMOVE[k];
                    if(insideBoard(x, y) && !piece[i][j].ally(piece[x][y])
                       && !isThatCheck(currentSide, make_pair(i, j), make_pair(x, y)))
                        addBit(movable[i][j], realPos(x, y));
                }
                break;
            default:{cerr << "Error: No piece detected\n"; throw;}
        }
    }
    if(!suitableMove){
        cerr << (inCheck ? "CHECKMATE!\n" : "Draw by Stalemate\n");
    }
}

//draw the board after each move
void Match::draw(){
    //Clear screen
    SDL_RenderClear( Game::instance().m_Renderer );

    //Render texture to screen
    board.draw();

    {//draw the squares
        if(!dMove.empty()){
            addColorSquare(dMove.back().from);
            addColorSquare(dMove.back().to);
        }
        if(hold_piece) addColorSquare(cur);
    }

    //prepare
    FU(i, 0, 8) FU(j, 0, 8)
        piece[i][j].setVal(board.getValue(i, j));
    //end prepare

    if(hold_piece){//draw the movable square
        Uint64 temp = movable[cur.fi][cur.se];
        short _ = -1;
        FU(i, 0, 8) FU(j, 0, 8){
            ++ _;
            if(!ONBIT(temp, _)){
                canMoveTo[i][j] = 0;
                continue;
            }
            canMoveTo[i][j] = 1;
            if(piece[i][j].getVal() == -1) dot1.draw(square[i][j]);
            else dot2.draw(square[i][j]);
        }
    }

    //draw the pieces
    FU(i, 0, 8) FU(j, 0, 8)
        piece[i][j].draw(square[i][j]);

    //Update screen
    SDL_RenderPresent( Game::instance().m_Renderer );
    SDL_Delay(20);

    if(reCalculate) calculate(), reCalculate = false;
}

//go to a new move
void Match::add_numMove(){
    reCalculate = true;
    cerr << numMove << endl;
    ++numMove;
    if(numMove & 1){
        ++numTurn;
        cerr << "Turns no. " << numTurn << endl;
    }
    currentSide ^= 1;
    if(currentSide == WHITE) cerr << "WHITE moves\n";
    else cerr << "BLACK moves\n";
}

//add color to a specific square
void Match::addColorSquare(pa X){
    SDL_SetRenderDrawColor(Game::instance().m_Renderer, 0xF6, 0xF6, 0x69, 0x8F);
    SDL_RenderFillRect(Game::instance().m_Renderer, &square[X.fi][X.se]);
}

void Match::addBit(Uint64 &x, int b){
    suitableMove = 1;
    UPBIT(x, b);
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
