#include "match.hpp"

int main( int argc, char* args[] ){
    // init game, in a badway

    SDL_SetRenderDrawBlendMode(Game::instance().m_Renderer, SDL_BLENDMODE_BLEND);
    Game::instance();
    Match match;
    match.mainEvent();

    Game::free();

	return 0;
}
