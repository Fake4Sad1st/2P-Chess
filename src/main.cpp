#include "match.hpp"

int main(int argc, char* args[] ){
    // init game, in a badway
    Game::instance();

    Match match;
    match.mainEvent();

    Game::free();

	return 0;
}
