#include "match.hpp"

int main(int argc, char* args[] ){
    // init game, in a badway
    Game::instance();

    SDL_SetRenderDrawBlendMode(Game::instance().m_Renderer, SDL_BLENDMODE_BLEND);
    Match match;
    match.mainEvent();

    Game::free();

	return 0;
}
