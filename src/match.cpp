#include "match.hpp"

#define ONBIT(x, i) ((x) >> (i) & 1)
#define UPBIT(x, i) ((x) |= (1ULL << (i)))

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
    promote = -1;
    currentSide = BLACK;
    cur = make_pair(-1, -1);
    board.initValue();
    FU(i, 0, 8) FU(j, 0, 8) hasMoved[i][j] = false;
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
        if( e.type == SDL_KEYDOWN ){ //reset field
            if( e.key.keysym.sym == SDLK_r ){init(); return;}
            if( e.key.keysym.sym == SDLK_ESCAPE ) promote = -1, hold_piece = false;
        }
        if( e.type == SDL_MOUSEBUTTONDOWN ) {
            int y_board = (e.motion.x < XOFF ? -1 : (e.motion.x - XOFF) / 80);
            int x_board = (e.motion.y < YOFF ? -1 : (e.motion.y - YOFF) / 80);
            if( !insideBoard(x_board, y_board) ) continue;
            cerr << x_board << ' ' << y_board << endl;
            if( e.button.button == SDL_BUTTON_LEFT ){
                if (hold_piece == true) {
                    if (promote != -1){
                        if(tempBoard[x_board][y_board] != -1){
                            hold_piece = false;
                            saveMove(cur, make_pair((currentSide == WHITE ? 0: 7), promote), tempBoard[x_board][y_board]);
                            promote = -1;
                            add_numMove();
                        }
                        else promote = -1, hold_piece = false;
                        continue;
                    }
                    hold_piece = false;
                    if(!canMoveTo[x_board][y_board]){
                        cerr << "CANCELLED!\n";
                        continue;
                    }
                    if(piece[cur.fi][cur.se].getVal() == PAWN && (x_board == 0 || x_board == 7)){
                        hold_piece = true;
                        create_promotionBoard(y_board);
                        continue;
                    }

                    saveMove(cur, make_pair(x_board, y_board));
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
            addColorSquare(COLOR_MOVED, dMove.back().from);
            addColorSquare(COLOR_MOVED, dMove.back().to);
        }
        if(hold_piece) addColorSquare(COLOR_MOVED, cur);
    }

    //prepare
    FU(i, 0, 8) FU(j, 0, 8){
        piece[i][j].setVal(board.getValue(i, j));
    }
    //end prepare

    if(promote != -1) drawPromotion();
    else if(hold_piece){//draw the movable square
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
        if(promote == -1 || tempBoard[i][j] == -1) piece[i][j].draw(square[i][j]);

    //Update screen
    SDL_RenderPresent( Game::instance().m_Renderer );
    SDL_Delay(20);

    if(reCalculate) calculate(), reCalculate = false;
}

//
void Match::create_promotionBoard(int col){
    promote = col;
    int dir = (currentSide == WHITE ? 1 : -1), coef = NUM_OF_PIECES * currentSide,
        row = (currentSide == WHITE ? 0 : 7);
    FU(i, 0, 8) FU(j, 0, 8) tempBoard[i][j] = -1;
    tempBoard[cur.fi][cur.se] = 0;
    for(int i = 0, k = coef + QUEEN; i < 4; ++i, --k){
        int cRow = row + dir*i;
        tempBoard[cRow][col] = k;
    }
}

//add the promotion
void Match::drawPromotion(){
    Texture T;
    FU(i, 0, 8) FU(j, 0, 8) if(tempBoard[i][j] > 0) {
        addColorSquare(COLOR_PROMOTE, make_pair(i, j));
        T = Texture(PieceLink[tempBoard[i][j]]);
        T.draw(square[i][j]);
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
    drawAnimation(dMove.back());
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
void Match::addColorSquare(Uint32 val, pa X){
    if(val == COLOR_MOVED) SDL_SetRenderDrawBlendMode(Game::instance().m_Renderer, SDL_BLENDMODE_BLEND);
    else SDL_SetRenderDrawBlendMode(Game::instance().m_Renderer, SDL_BLENDMODE_NONE);
    Uint8 a[4];
    for(int i = 3; i >= 0; i--) a[i] = val & 255, val >>= 8;
    SDL_SetRenderDrawColor(Game::instance().m_Renderer, a[0], a[1], a[2], a[3]);
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

//draw animation
void Match::drawAnimation(const Movement& X){
    int Ally = NUM_OF_PIECES * currentSide, Enemy = 6 - Ally;
    vector<Change> ani;
    if(X.speCase == NOTHING){
        ani.push_back(Change(X.from, X.to, Ally + X.pieceKind));
        if(X.pieceTaken != -1) ani.push_back(Change(X.to, Enemy + X.pieceTaken));
    }
    if(X.speCase == CASTLING){
        assert(X.pieceKind == KING);
        ani.push_back(Change(X.from, X.to, Ally + KING));
        int row = X.from.fi;
        if(X.to.se == 2) ani.push_back(Change(make_pair(row, 0), make_pair(row, 3), Ally + ROOK)); //o-o
        else ani.push_back(Change(make_pair(row, 7), make_pair(row, 5), Ally + ROOK)); //o-o-o
    }
    if(X.speCase == EN_PASSANT){
        assert(X.pieceKind == PAWN);
        ani.push_back(Change(X.from, X.to, Ally + PAWN));
        int row = X.from.fi, col = X.to.se;
        ani.push_back(Change(make_pair(row, col), Enemy + PAWN));
    }
    if(X.speCase == PROMOTING){
        ani.push_back(Change(X.from, X.to, -(Ally + X.promoteTo + 1)));
        if(X.pieceTaken != -1) ani.push_back(Change(X.to, Enemy + X.pieceTaken));
        return;
    }
    FU(i, 0, 8) FU(j, 0, 8) tempBoard[i][j] = -1;
    for(auto X: ani){
        tempBoard[X.from.fi][X.from.se] = 0;
        if(X.to.fi != -1) tempBoard[X.to.fi][X.to.se] = 0;
    }
    FU(i, 1, NUM_FRAME) drawAnimationStep(i, ani);
}

void Match::drawAnimationStep(int step, const vector<Change>& ani){
    //Clear screen
    SDL_RenderClear( Game::instance().m_Renderer );

    //Render texture to screen
    board.draw();

    {//draw the squares
        if(!dMove.empty()){
            addColorSquare(COLOR_MOVED, dMove.back().from);
            addColorSquare(COLOR_MOVED, dMove.back().to);
        }
        if(hold_piece) addColorSquare(COLOR_MOVED, cur);
    }

    //draw the pieces
    FU(i, 0, 8) FU(j, 0, 8){
        if(tempBoard[i][j] == -1) piece[i][j].draw(square[i][j]);
    }
    Texture T;
    for(auto X: ani){
        if(X.Case == MOVED){
            int xFrom = square[X.from.fi][X.from.se].x, yFrom = square[X.from.fi][X.from.se].y;
            int xTo = square[X.to.fi][X.to.se].x, yTo = square[X.to.fi][X.to.se].y;
            int _x = xFrom + (xTo - xFrom) / NUM_FRAME * step, _y = yFrom + (yTo - yFrom) / NUM_FRAME * step;
            T = Texture(PieceLink[X.pieceIn]);
            T.draw(_x, _y);
        }
        else{
            T = Texture(PieceLink[X.pieceIn]);
            Uint8 a = 16 * (NUM_FRAME - step);
            T.setAlpha(a);
            T.draw(square[X.from.fi][X.from.se]);
        }
    }

    //Update screen
    SDL_RenderPresent( Game::instance().m_Renderer );
    SDL_Delay(8);
}
