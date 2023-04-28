#include <SDL.h>
#include <SDL_image.h>
#include <bits/stdc++.h>

#include "chessboard.hpp"
#include "piece.hpp"

#define FU(i,a,b) for(int i=(a);i<(b);i++)
#define cout cerr

bool inside_board(int x, int y){
    return 0<=x && x<8 && 0<=y && y<8;
}

int main( int argc, char* args[] ){
    // init game, in a badway
    Game::instance();
    Chessboard board;
    board.initValue();
    Piece piece[8][8];
    FU(i, 0, 8) FU(j, 0, 8) piece[i][j].setPos(i, j);

    bool quit = false;
    pair<int, int> cur;
    SDL_Event e;
    bool hold_piece = false;
    while( !quit ){
        while( SDL_PollEvent( &e ) != 0 ) {
            if( e.type == SDL_QUIT ){ quit = true; break;}
            if( e.type == SDL_MOUSEBUTTONDOWN) {
                int x = e.motion.x;
                int y = e.motion.y;
                int y_board = static_cast<int>(std::floor(static_cast<float>(x - XOFF) / 80));
                int x_board = static_cast<int>(std::floor(static_cast<float>(y - YOFF) / 80));
                cerr << x << " " << y << " " << x_board << " " << y_board << "\n";
                if (!inside_board(x_board, y_board)) continue;
                if (hold_piece == true) {
                    hold_piece = false;
                    if(x_board == cur.first && y_board == cur.second){
                        cerr << "CANCELLED!" << endl;
                        continue;
                    }
                    if(piece[x_board][y_board].getVal() != -1 &&
                    piece[x_board][y_board].getSide() == piece[cur.first][cur.second].getSide()){
                        cerr << "Can't take the same side piece\n";
                        continue;
                    }

                    board.setValue(x_board, y_board, board.getValue(cur.first, cur.second));
                    board.setValue(cur.first, cur.second, -1);
                }
                else {
                    if (board.getValue(x_board, y_board) != -1) {
                        hold_piece = true;
                        cur = make_pair(x_board, y_board);
                    }
                }
            }
        }

        //Clear screen
        SDL_RenderClear( Game::instance().m_Renderer );

        //Render texture to screen
        board.draw();
        if (hold_piece) {
            SDL_Rect rect{XOFF + cur.second * PIECE_SIZE , YOFF + cur.first * PIECE_SIZE, PIECE_SIZE, PIECE_SIZE};
            SDL_SetRenderDrawColor(Game::instance().m_Renderer, 0xF6, 0xF6, 0x69, 0x8F);
            SDL_SetRenderDrawBlendMode(Game::instance().m_Renderer, SDL_BLENDMODE_BLEND);
            SDL_RenderFillRect(Game::instance().m_Renderer, &rect);
        }
        FU(i, 0, 8) FU(j, 0, 8){
            piece[i][j].setVal(board.getValue(i, j));
            piece[i][j].draw();
        }

        //Update screen
        SDL_RenderPresent( Game::instance().m_Renderer );
        SDL_Delay(20);
    }

    Game::free();

	return 0;
}
