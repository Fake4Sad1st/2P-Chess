#include <SDL.h>
#include <SDL_image.h>
#include <bits/stdc++.h>

#include "init.hpp"

SDL_Color on_color {0xff, 0, 0};
SDL_Rect A0={0,0,80,80}, A1={23,8+560,80,80};
int main( int argc, char* args[] ){
    try {
        Game game;
        Chessboard board(game);
        Texture Rook(game.gameRenderer, "img/piece/WhiteRook.png");

        bool quit = false;

        SDL_Event e;
        bool hold_piece = false;
        int xoff = 23, yoff = 8;
        int x_rook = 0, y_rook = 7;
        SDL_Rect {xoff + x_rook * 80, yoff + y_rook * 80};
        while( !quit ){
            while( SDL_PollEvent( &e ) != 0 ) {
                if( e.type == SDL_QUIT ) quit = true;
                if ( e.type == SDL_MOUSEBUTTONDOWN) {
                    int x = e.motion.x;
                    int y = e.motion.y;
                    int x_board = static_cast<int>(std::floor(static_cast<float>(x - xoff) / 80));
                    int y_board = static_cast<int>(std::floor(static_cast<float>(y - yoff) / 80));
                    cout << x << " " << y << " " << x_board << " " << y_board << "\n";
                    if (x_board < 0 || x_board > 7 || y_board < 0 || y_board > 7) continue;
                    if (hold_piece == true) {
                        x_rook = x_board;
                        y_rook = y_board;
                        hold_piece = false;
                    } else {
                        if (x_board == x_rook && y_board == y_rook) {
                            hold_piece = true;
                        }
                    }
                }
            }

            //Clear screen
            SDL_RenderClear( game.gameRenderer );

            //Render texture to screen

            board.draw();
            if (hold_piece) {
                SDL_Rect rect{xoff + x_rook * 80 +1, yoff + y_rook * 80+1, 80, 80};
                SDL_SetRenderDrawColor(game.gameRenderer, 0xF6, 0xF6, 0x69, 0x67);
//                SDL_SetRenderDrawBlendMode(game.gameRenderer, SDL_BLENDMODE_ADD);
                SDL_RenderFillRect(game.gameRenderer, &rect);
                SDL_SetRenderDrawBlendMode(game.gameRenderer, SDL_BLENDMODE_BLEND);
            }
            Rook.draw(xoff + x_rook * 80, yoff + y_rook * 80);

            //Update screen
            SDL_RenderPresent( game.gameRenderer );

        }
    } catch(std::exception& e) {}
	return 0;
}
