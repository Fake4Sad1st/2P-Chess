#include <SDL.h>
#include <SDL_image.h>
#include <bits/stdc++.h>

#include "chessboard.hpp"
#include "piece.hpp"

#define FU(i,a,b) for(int i=(a);i<(b);i++)

int main( int argc, char* args[] ){
    // init game, in a badway
    Game::instance();
    Chessboard board;
    board.initValue();
    Piece piece[8][8];
    FU(i, 0, 8) FU(j, 0, 8) piece[i][j].setPos(i, j);

    bool quit = false;

    SDL_Event e;
    bool hold_piece = false;
    while( !quit ){
        while( SDL_PollEvent( &e ) != 0 ) {
            if( e.type == SDL_QUIT ){ quit = true; break;}
            if( e.type == SDL_MOUSEBUTTONDOWN) {
                int x = e.motion.x;
                int y = e.motion.y;
                int x_board = static_cast<int>(std::floor(static_cast<float>(x - XOFF) / 80));
                int y_board = static_cast<int>(std::floor(static_cast<float>(y - YOFF) / 80));
                cerr << x << " " << y << " " << x_board << " " << y_board << "\n";
                if (x_board < 0 || x_board > 7 || y_board < 0 || y_board > 7) continue;
                if (hold_piece == true) {
//                    WPawn.setPos(x_board, y_board);
                    hold_piece = false;
                } else {
//                    if (x_board == WPawn.getx() && y_board == WPawn.gety()) {
//                        hold_piece = true;
//                    }
                }
            }
        }

        //Clear screen
        SDL_SetRenderDrawColor( Game::instance().m_Renderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( Game::instance().m_Renderer );

        //Render texture to screen

        board.draw();
        if (hold_piece) {
//            SDL_Rect rect{XOFF + WPawn.getx() * PIECE_SIZE , YOFF + WPawn.gety() * PIECE_SIZE, PIECE_SIZE, PIECE_SIZE};
//            SDL_SetRenderDrawColor(Game::instance().m_Renderer, 0xF6, 0xF6, 0x69, 0x67);
//            SDL_RenderFillRect(Game::instance().m_Renderer, &rect);
//            SDL_SetRenderDrawBlendMode(Game::instance().m_Renderer, SDL_BLENDMODE_BLEND);
        }
        FU(i, 0, 8) FU(j, 0, 8){
            piece[i][j].setVal(board.getValue(i, j));
            piece[i][j].draw();
        }

        //Update screen
        SDL_RenderPresent( Game::instance().m_Renderer );
    }

    Game::free();

	return 0;
}
