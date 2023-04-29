#include "match.hpp"

//about Movement class
Movement::Movement(){}

Movement::Movement(pa _from, pa _to, int _pieceKind, bool _takePiece):
    from(_from), to(_to), pieceKind(_pieceKind), takePiece(_takePiece) {}

//about Match class
Match::Match(){
    FU(i, 0, 8) FU(j, 0, 8)
        square[i][j] = SDL_Rect{XOFF + j * PIECE_SIZE , YOFF + i * PIECE_SIZE,
                                PIECE_SIZE, PIECE_SIZE};
    init();
}

Match::~Match(){}

void Match::init(){
    quit = hold_piece = false;
    currentSide = BLACK;
    numMove = numTurn = 0;
    board.initValue();
    FU(i, 0, 8) FU(j, 0, 8)
        piece[i][j].setPos(i, j);
    dMove.clear();
    draw();
}

bool inside_board(int x, int y){
    return 0<=x && x<8 && 0<=y && y<8;
}

void Match::mainEvent(){
    add_numMove();
    while(!quit){
        move();
        if(quit) break;
        draw();
    }
}

//detail of the move
void Match::move(){
    SDL_Event e;
    while( SDL_PollEvent( &e ) != 0 ) {
        if( e.type == SDL_QUIT ){ quit = true; return;}
        if( e.type == SDL_MOUSEBUTTONDOWN) {
            int x = e.motion.x;
            int y = e.motion.y;
            int y_board = (x < XOFF ? -1 : (x - XOFF) / 80);
            int x_board = (y < YOFF ? -1 : (y - YOFF) / 80);
            cerr << x << " " << y << " " << x_board << " " << y_board << "\n";
            if (!inside_board(x_board, y_board)) continue;
            if (hold_piece == true) {
                hold_piece = false;
                if(x_board == cur.fi && y_board == cur.se){
                    cerr << "CANCELLED!" << endl;
                    continue;
                }
                if(piece[cur.fi][cur.se].ally(piece[x_board][y_board])){
                    cerr << "Can't take the same side piece!\n";
                    continue;
                }

                dMove.push_back(Movement(cur, make_pair(x_board, y_board),
                                        piece[cur.fi][cur.se].getVal(), piece[x_board][y_board].isPiece()));
                board.setValue(x_board, y_board, board.getValue(cur.fi, cur.se));
                board.setValue(cur.fi, cur.se, -1);

                cerr << numMove << endl;
                add_numMove();
            }
            else {
                if (piece[x_board][y_board].getSide() == currentSide) {
                    hold_piece = true;
                    cur = make_pair(x_board, y_board);
                }
            }
        }
    }
}

//draw the board after each move
void Match::draw(){
    //Clear screen
    SDL_RenderClear( Game::instance().m_Renderer );

    //Render texture to screen
    board.draw();
        //draw the squares
    if(!dMove.empty()){
        addColorSquare(dMove.back().from);
        addColorSquare(dMove.back().to);
    }
    if(hold_piece) addColorSquare(cur);
        //
    FU(i, 0, 8) FU(j, 0, 8){
        piece[i][j].setVal(board.getValue(i, j));
        piece[i][j].draw();
    }

    //Update screen
    SDL_RenderPresent( Game::instance().m_Renderer );
    SDL_Delay(20);
}

//go to a new move
void Match::add_numMove(){
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